#include "ServerPacketTypeManager.h"
#include "NetworkManager.h"
#include <iostream>
#include "MovementPacket.h"
#include "ThreadManager.h"
#include <thread>

sf::Packet& operator>>(sf::Packet& packet, PacketTypes& type) {
	int temp;
	packet >> temp;
	type = static_cast<PacketTypes>(temp);

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, PacketTypes& type) {
	int temp;
	temp = static_cast<int>(type);
	packet << temp;

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, UdpPacketTypes& type) {
	int temp;
	packet >> temp;
	type = static_cast<UdpPacketTypes>(temp);

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, UdpPacketTypes& type) {
	int temp;
	temp = static_cast<int>(type);
	packet << temp;

	return packet;
}

void ServerPacketTypesManager::ReceivePacket(sf::Packet packet, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort)
{
	uint8_t receivedDataPriority;

	packet >> receivedDataPriority;

	if (receivedDataPriority == 0)
	{
		std::cout << "Normal" << std::endl;
	}
	else
	{
		if (receivedDataPriority & URGENT_PACKET)
			std::cout << "Urgente" << std::endl;

		if (receivedDataPriority & CRITICAL_PACKET)
			std::cout << "Crítico" << std::endl;
	}

	if (receivedDataPriority & URGENT_PACKET)
	{
		ThrdM->AddUrgentTask(new Task([this, packet]() mutable {
			this->ProcessPacket(packet);
			}));
	}
	else
	{
		ThrdM->AddTask(new Task([this, packet]() mutable {
			this->ProcessPacket(packet);
			}));
	}
}

void ServerPacketTypesManager::SendData(sf::TcpSocket& client, sf::Packet& packet)
{
	if (client.send(packet) == sf::Socket::Status::Done) {
		std::cout << "Enviando paquete..." << std::endl;
	}
	else {
		std::cerr << "Error al enviar el paquete" << std::endl;
	}
}

void ServerPacketTypesManager::SendHandshake(sf::TcpSocket& client)
{
	sf::Packet packet;
	packet << PacketTypes::HANDSHAKE << handshakeMessage;
	SendData(client, packet);
}

void ServerPacketTypesManager::SendUpdatedPlayerCount(sf::TcpSocket& client, int playerCount)
{
	sf::Packet packet;
	packet << PacketTypes::WAITING_ROOM_PLAYERS;
	packet << playerCount;

	SendData(client, packet);
}

void ServerPacketTypesManager::SendLobbyCreateResponse(sf::TcpSocket& client, bool success)
{
	sf::Packet packet;
	packet << PacketTypes::LOBBY_CREATE;
	packet << success;

	SendData(client, packet);

	std::cout << "Respuesta de creacion de lobby enviada" << std::endl;
}

void ServerPacketTypesManager::SendLobbyJoinResponse(sf::TcpSocket& client, bool success)
{
	sf::Packet packet;
	packet << PacketTypes::LOBBY_JOIN;
	packet << success;

	SendData(client, packet);

	std::cout << "Respuesta de join a lobby enviada" << std::endl;
}

void ServerPacketTypesManager::ProcessPacket(sf::Packet packet)
{
	UdpPacketTypes packetType;

	packet >> packetType;

	switch (packetType)
	{
	case MOVEMENT:
		ReceiveMovementPacket(packet);
		break;
	default:
		std::cout << "No se ha identificado el tipo de packete" << std::endl;
		break;
	}

	packet.clear();
}

void ServerPacketTypesManager::ReceiveHandshakePacket(sf::Packet data)
{
	std::string receiveMesage;
	data >> receiveMesage;

	std::cout << "Mensaje enviado del cliente: " << receiveMesage << std::endl;
}

void ServerPacketTypesManager::ReceiveMovementPacket(sf::Packet data)
{
	MovementPacket movement;
	data >> movement;

	std::cout << "Recibido movimiento: ";
	std::cout << "ID " << movement.ID << " | ";
	std::cout << "(" << movement.pos.x << ", " << movement.pos.y << ")" << std::endl;
}

void ServerPacketTypesManager::ReceiveStartGamePacket(sf::Packet data)
{
}

void ServerPacketTypesManager::ReceiveEndGamePacket(sf::Packet data)
{
}
