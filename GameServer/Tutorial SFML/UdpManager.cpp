#include "UdpManager.h" 
#include "ThreadManager.h" 
#include "GameRoomManager.h"
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

void UdpManager::SendCriticalPacket(const sf::IpAddress& ip, unsigned short port, int id, sf::Packet packet) {
	pendingCriticalPacketsToSend.push_back({ id, packet, ip, port });
	SendData(ip, port, packet);
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

void UdpManager::ProcessedCriticalPacket(const std::string& key, const sf::IpAddress& ip, unsigned short port, int id) {
	processedCriticalPackets[key].insert(id);
	SendAcknowledgement(ip, port, id);
}

void UdpManager::SendAcknowledgement(const sf::IpAddress& ip, unsigned short port, int id) {
	sf::Packet packet;
	uint8_t priority = NORMAL_PACKET;

	packet << priority;
	packet << PacketType::ACKNOWLEDGEMENT;
	packet << id;

	SendData(ip, port, packet);
}

void UdpManager::SendData(const sf::IpAddress& ip, unsigned short port, const sf::Packet& packet) {
	const void* data = packet.getData();
	std::size_t dataSize = packet.getDataSize();

	if (socket.send(data, dataSize, ip, port) == sf::Socket::Status::Done)
	{
		// std::cout << "Paquete UDP enviado..." << std::endl;
	}
	else
	{
		std::cerr << "Error al enviar paquete UDP" << std::endl;
	}
}

void UdpManager::ProcessPacket(PacketType type, sf::Packet data, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort) {
	if (!senderIp.has_value()) return;

	switch (type) {
	case PacketType::MOVEMENT:
		ReceiveMovement(data, senderIp.value(), senderPort);
		break;
	case PacketType::SHOT:
		ReceiveShot(data, senderIp.value(), senderPort);
		break;
	case PacketType::TAUNT:
		ReceiveTaunt(senderIp.value(), senderPort);
		break;
	case PacketType::MATCH_CONNECT:
		ReceiveMatchConnect(data, senderIp.value(), senderPort);
		break;
	default:
		std::cout << "No se ha identificado el tipo de paquete udp" << std::endl;
		break;
	}
}

void UdpManager::ReceiveMovement(sf::Packet data, const sf::IpAddress& ip, unsigned short port) {
	MovementPacket movement;
	data >> movement;
	GRM->HandleMovement(ip, port, movement);
}

void UdpManager::ReceiveShot(sf::Packet data, const sf::IpAddress& ip, unsigned short port) {
	bool facingRight;
	data >> facingRight;
	GRM->HandleShot(ip, port, facingRight);
}

void UdpManager::ReceiveTaunt(const sf::IpAddress& ip, unsigned short port) {
	std::cout << "Recibida burla de " << ip.toString() << ":" << port << std::endl;

	GRM->HandleTaunt(ip, port);
}

void UdpManager::ReceiveMatchConnect(sf::Packet data, const sf::IpAddress& ip, unsigned short port) {
	int roomId;
	uint8_t playerIndex;

	data >> roomId;
	data >> playerIndex;

	std::cout << "Recibida conexion de player " << (int)playerIndex << " para la sala " << roomId << std::endl;

	GRM->ConnectPlayerToRoom(roomId, playerIndex, ip, port);
}

bool UdpManager::Init() {
	bool successful = true;

	socket.setBlocking(false);

	if (socket.bind(BIND_PORT) != sf::Socket::Status::Done)
	{
		std::cout << "Error al abrir socket UDP del cliente" << std::endl;
		successful = false;
	}

	return successful;
}

void UdpManager::AttemptToSendPendingCriticalPackets() {
	for (const PendingCriticalPacket& criticalPacket : pendingCriticalPacketsToSend) {
		SendData(criticalPacket.ip, criticalPacket.port, criticalPacket.packet);
	}
}

void UdpManager::ReceivePacket() {
	char buffer[1024];
	std::size_t receivedSize;
	std::optional<sf::IpAddress> senderIp;
	unsigned short senderPort;

	if (socket.receive(buffer, sizeof(buffer), receivedSize, senderIp, senderPort) == sf::Socket::Status::Done)
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
				SendAcknowledgement(senderIp.value(), senderPort, criticalId);
				packet.clear();
				return;
			}
			else
			{
				ProcessedCriticalPacket(clientKey, senderIp.value(), senderPort, criticalId);
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
			if (priority & URGENT_PACKET)
			{
				ThrdM->AddUrgentTask(new Task([this, packetType, packet, senderIp, senderPort]() mutable {
					this->ProcessPacket(packetType, packet, senderIp, senderPort);
					}));
			}
			else
			{
				ThrdM->AddTask(new Task([this, packetType, packet, senderIp, senderPort]() mutable {
					this->ProcessPacket(packetType, packet, senderIp, senderPort);
					}));
			}
		}

		packet.clear();
	}
}

void UdpManager::SendMatchStart(const sf::IpAddress& ip, unsigned short port) {
	sf::Packet packet;
	uint8_t priority = CRITICAL_PACKET;
	int id = GetNextCriticalPacketId();

	packet << priority;
	packet << id;
	packet << PacketType::MATCH_START;

	SendCriticalPacket(ip, port, id, packet);
	std::cout << "MATCH_START enviado a " << ip.toString() << ":" << port << std::endl;
}

void UdpManager::SendHealthUpdate(const sf::IpAddress& ip, unsigned short port, uint8_t playerIndex, int health, int lives) {
	sf::Packet packet;
	uint8_t priority = CRITICAL_PACKET;
	int id = GetNextCriticalPacketId();

	packet << priority;
	packet << id;
	packet << PacketType::HEALTH_UPDATE;

	packet << playerIndex;
	packet << static_cast<std::int32_t>(health);
	packet << static_cast<std::int32_t>(lives);

	SendCriticalPacket(ip, port, id, packet);
}

void UdpManager::SendMovement(MovementPacket movement) {}
void UdpManager::SendShot(bool towardsRight) {}

void UdpManager::SendTaunt(const sf::IpAddress& ip, unsigned short port)
{
	sf::Packet packet;
	uint8_t priority = URGENT_PACKET;

	packet << priority;
	packet << PacketType::TAUNT;
	std::cout << "Burla enviada a " << ip.toString() << ":" << port << std::endl;

	SendData(ip, port, packet);
}