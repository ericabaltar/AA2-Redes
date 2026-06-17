#include "UdpManager.h" 
#include "NetworkManager.h" 
#include "MatchManager.h" 
#include <iostream>
#include <cstdint>

std::string UdpManager::MakeClientKey(const sf::IpAddress& ip, unsigned short port) {
	return ip.toString() + ":" + std::to_string(port);
}

int UdpManager::GetNextCriticalPacketId() {
	currentCriticalPacketId++;
	return currentCriticalPacketId;
}

void UdpManager::SendCriticalPacket(int id, char* buffer, size_t bufferSize) {
	pendingCriticalPacketsToSend.push_back({ id, buffer, bufferSize });
	SendData(buffer, bufferSize);
}

void UdpManager::AttemptToSendPendingCriticalPackets() {
	for (const PendingCriticalPacket& packet : pendingCriticalPacketsToSend) {
		SendData(packet.buffer, packet.bufferSize);
	}
}

void UdpManager::ReceivePacket() {
	char buffer[PACKET_SIZE];
	size_t dataRead = 0;
	std::size_t receivedSize;
	std::optional<sf::IpAddress> senderIp;
	unsigned short senderPort;

	if (socket.receive(buffer, sizeof(buffer), receivedSize, senderIp, senderPort) == sf::Socket::Status::Done)
	{
		uint8_t priority;
		std::memcpy(&priority, buffer + dataRead, sizeof(priority));
		dataRead += sizeof(priority);

		bool isCritical = (priority & CRITICAL_PACKET) != 0;
		int criticalId = 0;

		if (isCritical) {
			std::memcpy(&criticalId, buffer + dataRead, sizeof(criticalId));
			dataRead += sizeof(criticalId);
		}

		PacketType packetType;
		std::memcpy(&packetType, buffer + dataRead, sizeof(packetType));
		dataRead += sizeof(packetType);

		if (isCritical)
		{
			std::string clientKey = MakeClientKey(senderIp.value(), senderPort);

			if (PacketIsAlreadyProcessed(clientKey, criticalId))
			{
				SendAcknowledgement(criticalId);
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
			std::memcpy(&id, buffer + dataRead, sizeof(id));
			dataRead += sizeof(id);
			RemoveCriticalPacketFromPending(id);
		}
		else
		{
			ProcessPacket(packetType, buffer, dataRead);
		}
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

void UdpManager::SendAcknowledgement(int id)
{
	char buffer[PACKET_SIZE];
	size_t bufferSize = 0;

	uint8_t priority = NORMAL_PACKET;
	PacketType type = PacketType::ACKNOWLEDGEMENT;

	std::memcpy(buffer + bufferSize, &priority, sizeof(priority));
	bufferSize += sizeof(priority);

	std::memcpy(buffer + bufferSize, &type, sizeof(type));
	bufferSize += sizeof(type);

	std::memcpy(buffer + bufferSize, &id, sizeof(id));
	bufferSize += sizeof(id);

	SendData(buffer, bufferSize);
}

void UdpManager::SendData(char* buffer, size_t size)
{
	if (socket.send(buffer, size, SERVER_IP, SERVER_PORT) == sf::Socket::Status::Done)
	{
		// std::cout << "Paquete UDP enviado..." << std::endl;
	}
	else
	{
		std::cerr << "Error al enviar paquete UDP" << std::endl;
	}
}

void UdpManager::ProcessPacket(PacketType type, char* buffer, size_t dataRead) {
	switch (type) {
	case PacketType::MOVEMENT:
		ReceiveMovement(buffer, dataRead);
		break;
	case PacketType::SHOT:
		ReceiveShot(buffer, dataRead);
		break;
	case PacketType::TAUNT:
		ReceiveTaunt(buffer, dataRead);
		break;
	case PacketType::HEALTH_UPDATE:
		ReceiveHealthUpdate(buffer, dataRead);
		break;
	case PacketType::MATCH_START:
		ReceiveMatchStart(buffer, dataRead);
		break;
	case PacketType::PING:
		ReceivePing(buffer, dataRead);
		break;
	case PacketType::DISCONNECT:
		ReceiveDisconnect(buffer, dataRead);
		break;
	default:
		std::cout << "No se ha identificado el tipo de paquete de udp" << std::endl;
		break;
	}
}

void UdpManager::ReceiveMovement(char* buffer, size_t dataRead) {
	MovementPacket movementPacket;

	std::memcpy(&movementPacket, buffer + dataRead, sizeof(movementPacket));
	dataRead += sizeof(movementPacket);

	NT->SetLastValidatedMovementPacket(movementPacket);
}

void UdpManager::ReceiveShot(char* buffer, size_t dataRead) {}

void UdpManager::ReceiveTaunt(char* buffer, size_t dataRead)
{
	std::cout << "Recibida burla" << std::endl;
	MM->HandleEnemyQuack();
}

void UdpManager::ReceiveHealthUpdate(char* buffer, size_t dataRead) {
	uint8_t playerIndex;
	int health, lives;

	std::memcpy(&playerIndex, buffer + dataRead, sizeof(playerIndex));
	dataRead += sizeof(playerIndex);

	std::memcpy(&health, buffer + dataRead, sizeof(health));
	dataRead += sizeof(health);

	std::memcpy(&lives, buffer + dataRead, sizeof(lives));
	dataRead += sizeof(lives);

	NT->QueueHealthUpdate(playerIndex, health, lives);
}

void UdpManager::ReceiveMatchStart(char* buffer, size_t dataRead) {
	std::cout << "Se han conectado ambos clientes. MATCH START recibido. Iniciando gameplay..." << std::endl;
	MM->StartGame();
}

void UdpManager::ReceivePing(char* buffer, size_t dataRead)
{
	SendPing();
}

void UdpManager::ReceiveDisconnect(char* buffer, size_t dataRead)
{
	MM->EndGame();
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
	
	char buffer[PACKET_SIZE];
	size_t bufferSize = 0;

	uint8_t priority = CRITICAL_PACKET;
	int id = GetNextCriticalPacketId();
	PacketType type = PacketType::MATCH_CONNECT;

	std::memcpy(buffer + bufferSize, &priority, sizeof(priority));
	bufferSize += sizeof(priority);

	std::memcpy(buffer + bufferSize, &id, sizeof(id));
	bufferSize += sizeof(id);

	std::memcpy(buffer + bufferSize, &type, sizeof(type));
	bufferSize += sizeof(type);

	std::memcpy(buffer + bufferSize, &roomId, sizeof(roomId));
	bufferSize += sizeof(roomId);

	std::memcpy(buffer + bufferSize, &playerIndex, sizeof(playerIndex));
	bufferSize += sizeof(playerIndex);

	SendCriticalPacket(id, buffer, bufferSize);

	std::cout << "Enviada peticion de conexion a match " << roomId << " como player " << static_cast<int>(playerIndex) << std::endl;
}

void UdpManager::SendMovement(MovementPacket movement)
{
	char buffer[PACKET_SIZE];
	size_t bufferSize = 0;

	uint8_t priority = NORMAL_PACKET;
	PacketType type = PacketType::MOVEMENT;

	std::memcpy(buffer + bufferSize, &priority, sizeof(priority));
	bufferSize += sizeof(priority);

	std::memcpy(buffer + bufferSize, &type, sizeof(type));
	bufferSize += sizeof(type);

	std::memcpy(buffer + bufferSize, &movement, sizeof(movement));
	bufferSize += sizeof(movement);

	SendData(buffer, bufferSize);
}

void UdpManager::SendShot(bool towardsRight) {

	char buffer[PACKET_SIZE];
	size_t bufferSize = 0;

	uint8_t priority = CRITICAL_PACKET;
	int id = GetNextCriticalPacketId();
	PacketType type = PacketType::SHOT;

	std::memcpy(buffer + bufferSize, &priority, sizeof(priority));
	bufferSize += sizeof(priority);

	std::memcpy(buffer + bufferSize, &id, sizeof(id));
	bufferSize += sizeof(id);

	std::memcpy(buffer + bufferSize, &type, sizeof(type));
	bufferSize += sizeof(type);

	std::memcpy(buffer + bufferSize, &towardsRight, sizeof(towardsRight));
	bufferSize += sizeof(towardsRight);

	SendCriticalPacket(id, buffer, bufferSize);
}

void UdpManager::SendTaunt()
{
	char buffer[PACKET_SIZE];
	size_t bufferSize = 0;

	uint8_t priority = URGENT_PACKET;
	PacketType type = PacketType::TAUNT;

	std::memcpy(buffer + bufferSize, &priority, sizeof(priority));
	bufferSize += sizeof(priority);

	std::memcpy(buffer + bufferSize, &type, sizeof(type));
	bufferSize += sizeof(type);

	SendData(buffer, bufferSize);
}

void UdpManager::SendPing()
{
	char buffer[PACKET_SIZE];
	size_t bufferSize = 0;

	uint8_t priority = URGENT_PACKET;
	PacketType type = PacketType::PING;

	std::memcpy(buffer + bufferSize, &priority, sizeof(priority));
	bufferSize += sizeof(priority);

	std::memcpy(buffer + bufferSize, &type, sizeof(type));
	bufferSize += sizeof(type);

	SendData(buffer, bufferSize);
}
