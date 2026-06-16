#include "UdpManager.h" 
#include "NetworkManager.h" 
#include "MatchManager.h" 
#include <iostream>
#include <cstdint>

sf::Packet& operator>>(sf::Packet& packet, UdpManager::PacketType& type) {
	uint8_t temp;
	packet >> temp;
	type = static_cast<UdpManager::PacketType>(temp);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, UdpManager::PacketType type) {
	uint8_t temp;
	temp = static_cast<uint8_t>(type);
	packet << temp;
	return packet;
}

std::string UdpManager::MakeClientKey(const sf::IpAddress& ip, unsigned short port) {
	return ip.toString() + ":" + std::to_string(port);
}

int UdpManager::GetNextCriticalPacketId() {
	currentCriticalPacketId++;
	return currentCriticalPacketId;
}

void UdpManager::SendCriticalPacket(int id, sf::Packet packet)
{
	pendingCriticalPacketsToSend.push_back({
		id,
		packet,
		sf::seconds(0.f)
		});

	SendData(packet);

	pendingCriticalPacketsToSend.back().lastSendTime = resendClock.getElapsedTime();
}


void UdpManager::AttemptToSendPendingCriticalPackets()
{
	sf::Time now = resendClock.getElapsedTime();

	for (PendingCriticalPacket& criticalPacket : pendingCriticalPacketsToSend)
	{
		if ((now - criticalPacket.lastSendTime).asMilliseconds() >= criticalPacketCooldown)
		{
			SendData(criticalPacket.packet);
			criticalPacket.lastSendTime = now;
		}
	}
}

void UdpManager::ReceivePacket() {
	char buffer[1024];
	std::size_t receivedSize;
	std::optional<sf::IpAddress> senderIp;
	unsigned short senderPort;

	while (socket.receive(buffer, sizeof(buffer), receivedSize, senderIp, senderPort) == sf::Socket::Status::Done)
	{
		sf::Packet packet;
		packet.append(buffer, receivedSize);

		uint8_t priority;
		packet >> priority;

		PacketType packetType;

		bool isCritical = (priority & CRITICAL_PACKET) != 0;
		int criticalId = 0;

		if (isCritical)
			packet >> criticalId;

		packet >> packetType;

		if (isCritical)
		{
			std::string clientKey = MakeClientKey(senderIp.value(), senderPort);

			if (PacketIsAlreadyProcessed(clientKey, criticalId))
			{
				SendAcknowledgement(criticalId);
				packet.clear();
				return;
			}
			else
			{
				ProcessedCriticalPacket(clientKey, criticalId);
			}
		}

		if (packetType == PacketType::ACKNOWLEDGEMENT)
		{
			int id;
			packet >> id;
			RemoveCriticalPacketFromPending(id);
		}
		else
		{
			ProcessPacket(packetType, packet);
		}

		packet.clear();
	}
}

void UdpManager::RemoveCriticalPacketFromPending(int id) {
	for (std::vector<PendingCriticalPacket>::iterator it = pendingCriticalPacketsToSend.begin(); it != pendingCriticalPacketsToSend.end(); it++) {
		if (it->id == id) {
			pendingCriticalPacketsToSend.erase(it);
			return;
		}
	}
}

bool UdpManager::PacketIsAlreadyProcessed(const std::string& key, int id) {
	std::unordered_map<std::string, std::unordered_set<int>>::iterator mapIt;
	mapIt = processedCriticalPackets.find(key);

	if (mapIt == processedCriticalPackets.end())
		return false;

	std::unordered_set<int>::iterator setIt = mapIt->second.find(id);

	return setIt != mapIt->second.end();
}

void UdpManager::ProcessedCriticalPacket(const std::string& key, int id) {
	processedCriticalPackets[key].insert(id);
	SendAcknowledgement(id);
}

void UdpManager::SendAcknowledgement(int id) {
	sf::Packet packet;
	uint8_t priority = NORMAL_PACKET;

	packet << priority;
	packet << PacketType::ACKNOWLEDGEMENT;
	packet << id;

	SendData(packet);
}

void UdpManager::SendData(const sf::Packet& packet) {
	const void* data = packet.getData();
	std::size_t dataSize = packet.getDataSize();

	if (socket.send(data, dataSize, SERVER_IP, SERVER_PORT) == sf::Socket::Status::Done)
	{
		// std::cout << "Paquete UDP enviado..." << std::endl;
	}
	else
	{
		std::cerr << "Error al enviar paquete UDP" << std::endl;
	}
}

void UdpManager::ProcessPacket(PacketType type, sf::Packet data)
{
	switch (type) {
	case PacketType::MOVEMENT:
		ReceiveMovement(data);
		break;
	case PacketType::SHOT:
		ReceiveShot(data);
		break;
	case PacketType::TAUNT:
		ReceiveTaunt(data);
		break;
	case PacketType::HEALTH_UPDATE:
		ReceiveHealthUpdate(data);
		break;
	case PacketType::MATCH_START:
		ReceiveMatchStart(data);
		break;
	default:
		std::cout << "No se ha identificado el tipo de paquete de udp" << std::endl;
		break;
	}
}

void UdpManager::ReceiveMovement(sf::Packet data) {
	MovementPacket movementPacket;
	data >> movementPacket;
	NT->SetLastValidatedMovementPacket(movementPacket);
}

void UdpManager::ReceiveShot(sf::Packet data)
{
	bool towardsRight;
	data >> towardsRight;

	std::cout << "Recibido disparo" << std::endl;
	MM->HandleEnemyShot(towardsRight);
}

void UdpManager::ReceiveTaunt(sf::Packet data)
{
	std::cout << "Recibida burla" << std::endl;
	MM->HandleEnemyQuack();
}

void UdpManager::ReceiveHealthUpdate(sf::Packet data) {
	uint8_t playerIndex;
	std::int32_t health, lives;
	data >> playerIndex >> health >> lives;

	NT->QueueHealthUpdate(playerIndex, static_cast<int>(health), static_cast<int>(lives));
}

void UdpManager::ReceiveMatchStart(sf::Packet data) {
	std::cout << "Se han conectado ambos clientes. MATCH START recibido. Iniciando gameplay..." << std::endl;
	MM->StartGame();
}

bool UdpManager::Init() {
	bool successful = true;
	socket.setBlocking(false);

	if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
	{ 
		std::cout << "Error al abrir socket UDP del cliente" << std::endl;
		successful = false;
	}

	return successful;
}

void UdpManager::SendMatchConnect(int roomId, uint8_t playerIndex) {
	sf::Packet packet;
	uint8_t priority = CRITICAL_PACKET;
	int id = GetNextCriticalPacketId();

	packet << priority;
	packet << id;
	packet << PacketType::MATCH_CONNECT;
	packet << roomId;
	packet << playerIndex;

	std::cout << "Enviada peticion de conexion a match " << roomId << " como player " << static_cast<int>(playerIndex) << std::endl;

	SendCriticalPacket(id, packet);
}

void UdpManager::SendMovement(MovementPacket movement) {
	sf::Packet packet;
	uint8_t priority = NORMAL_PACKET;

	packet << priority;
	packet << PacketType::MOVEMENT;
	packet << movement;

	//SendData(packet);
}

void UdpManager::SendShot(bool towardsRight)
{
	sf::Packet packet;
	uint8_t priority = CRITICAL_PACKET | URGENT_PACKET;
	int id = GetNextCriticalPacketId();

	packet << priority;
	packet << id;
	packet << PacketType::SHOT;
	packet << towardsRight;

	SendCriticalPacket(id, packet);
}

void UdpManager::SendTaunt()
{
	sf::Packet packet;
	uint8_t priority = URGENT_PACKET;

	packet << priority;
	packet << PacketType::TAUNT;

	SendData(packet);
}