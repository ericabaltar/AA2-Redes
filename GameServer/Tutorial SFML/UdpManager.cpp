#include "UdpManager.h" 
#include "ThreadManager.h" 
#include "GameRoomManager.h"
#include <iostream>

sf::Packet& operator>>(sf::Packet& packet, UdpManager::PacketType& type)
{
	uint8_t temp;
	packet >> temp;
	type = static_cast<UdpManager::PacketType>(temp);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, UdpManager::PacketType type)
{
	uint8_t temp;
	temp = static_cast<uint8_t>(type);
	packet << temp;
	return packet;
}

std::string UdpManager::MakeClientKey(const sf::IpAddress& ip, unsigned short port)
{
	return ip.toString() + ":" + std::to_string(port);
}

int UdpManager::GetNextCriticalPacketId()
{
	currentCriticalPacketId++;
	return currentCriticalPacketId;
}

void UdpManager::SendCriticalPacket(const sf::IpAddress& ip, unsigned short port, int id, sf::Packet packet)
{
	pendingCriticalPacketsToSend.push_back({ id, packet, ip, port });
	SendData(ip, port, packet);
}

void UdpManager::RemoveCriticalPacketFromPending(int id)
{
	for (std::vector<PendingCriticalPacket>::iterator it = pendingCriticalPacketsToSend.begin(); it != pendingCriticalPacketsToSend.end(); it++) {
		if (it->id == id) {
			pendingCriticalPacketsToSend.erase(it);
			return;
		}
	}
}

bool UdpManager::PacketIsAlreadyProcessed(const std::string& key, int id)
{
	std::unordered_map<std::string, std::unordered_set<int>>::iterator mapIt;
	mapIt = processedCriticalPackets.find(key);

	if (mapIt == processedCriticalPackets.end())
		return false;

	std::unordered_set<int>::iterator setIt = mapIt->second.find(id);

	return setIt != mapIt->second.end();
}

void UdpManager::ProcessedCriticalPacket(const std::string& key, const sf::IpAddress& ip, unsigned short port, int id)
{
	processedCriticalPackets[key].insert(id);
	SendAcknowledgement(ip, port, id);
}

void UdpManager::SendAcknowledgement(const sf::IpAddress& ip, unsigned short port, int id)
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

	SendData(ip, port, buffer, bufferSize);
}

void UdpManager::SendData(const sf::IpAddress& ip, unsigned short port, char* buffer, size_t size)
{
	if (socket.send(buffer, size, ip, port) == sf::Socket::Status::Done)
	{
		// std::cout << "Paquete UDP enviado..." << std::endl;
	}
	else
	{
		std::cerr << "Error al enviar paquete UDP" << std::endl;
	}
}

void UdpManager::ProcessPacket(PacketType type, char* buffer, size_t dataRead, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort)
{
	switch (type) {
	case PacketType::MOVEMENT:
		ReceiveMovement(buffer, dataRead);
		break;
	case PacketType::SHOT:
		ReceiveShot(data);
		break;
	case PacketType::TAUNT:
		ReceiveTaunt(data);
		break;
	case PacketType::MATCH_CONNECT:
		ReceiveMatchConnect(buffer, dataRead, senderIp.value(), senderPort);
		break;
	default:
		std::cout << "No se ha identificado el tipo de paquete udp" << std::endl;
		break;
	}
}

void UdpManager::ReceiveMovement(char* buffer, size_t dataRead) {}

void UdpManager::ReceiveShot(sf::Packet data) {}

void UdpManager::ReceiveTaunt(sf::Packet data) {}

void UdpManager::ReceiveMatchConnect(char* buffer, size_t dataRead, const sf::IpAddress& ip, unsigned short port)
{
	int roomId;
	uint8_t playerIndex;

	std::memcpy(&roomId, buffer + dataRead, sizeof(roomId));
	dataRead += sizeof(roomId);
	
	std::memcpy(&playerIndex, buffer + dataRead, sizeof(playerIndex));
	dataRead += sizeof(playerIndex);

	std::cout << "Recibida conexion de player " << (int)playerIndex << " para la sala " << roomId << std::endl;

	GRM->ConnectPlayerToRoom(roomId, playerIndex, ip, port);
}

bool UdpManager::Init()
{
	bool successful = true;

	socket.setBlocking(false);

	if (socket.bind(BIND_PORT) != sf::Socket::Status::Done)
	{
		std::cout << "Error al abrir socket UDP del cliente" << std::endl;
		successful = false;
	}

	return successful;
}

void UdpManager::AttemptToSendPendingCriticalPackets()
{
	for (const PendingCriticalPacket& criticalPacket : pendingCriticalPacketsToSend) {
		SendData(criticalPacket.ip, criticalPacket.port, criticalPacket.packet);
	}
}

void UdpManager::ReceivePacket()
{
	char buffer[PACKET_SIZE];
	size_t dataRead = 0;
	size_t receivedSize;
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
				SendAcknowledgement(senderIp.value(), senderPort, criticalId);
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
			std::memcpy(&id, buffer + dataRead, sizeof(id));
			dataRead += sizeof(id);
			RemoveCriticalPacketFromPending(id);
		}
		else
		{
			if (priority & URGENT_PACKET)
			{
				ThrdM->AddUrgentTask(new Task([this, packetType, buffer, dataRead, senderIp, senderPort]() mutable {
					this->ProcessPacket(packetType, buffer, dataRead, senderIp, senderPort);
					}));
			}
			else
			{
				ThrdM->AddTask(new Task([this, packetType, buffer, dataRead, senderIp, senderPort]() mutable {
					this->ProcessPacket(packetType, buffer, dataRead, senderIp, senderPort);
					}));
			}
		}
	}
}

void UdpManager::SendMatchStart(const sf::IpAddress& ip, unsigned short port)
{
	sf::Packet packet;
	uint8_t priority = CRITICAL_PACKET;
	int id = GetNextCriticalPacketId();

	packet << priority;
	packet << id;
	packet << PacketType::MATCH_START;

	SendCriticalPacket(ip, port, id, packet);
	std::cout << "MATCH_START enviado a " << ip.toString() << ":" << port << std::endl;
}