#include "ServerPacketTypeManager.h"
#include <iostream>

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

void ServerPacketTypesManager::ReceivePacket(sf::Packet packet, sf::IpAddress& senderIp, unsigned short senderPort)
{
	PacketTypes packetType;

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

void ServerPacketTypesManager::SendLoginResponse(sf::TcpSocket& client, bool success, const std::string& username)
{
	sf::Packet packet;
	packet << PacketTypes::LOGIN;
	packet << success;
	packet << username;

	SendData(client, packet);

	std::cout << "Respuesta de login enviada" << std::endl;
}

void ServerPacketTypesManager::SendRegisterResponse(sf::TcpSocket& client, bool success, const std::string& username)
{
	sf::Packet packet;
	packet << PacketTypes::REGISTER;
	packet << success;
	packet << username;

	SendData(client, packet);

	std::cout << "Respuesta de registro enviada" << std::endl;
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

void ServerPacketTypesManager::ReceiveHandshakePacket(sf::Packet data)
{
	std::string receiveMesage;
	data >> receiveMesage;

	std::cout << "Mensaje enviado del cliente: " << receiveMesage << std::endl;
}

void ServerPacketTypesManager::ReceiveMovementPacket(sf::Packet data)
{
}

void ServerPacketTypesManager::ReceiveStartGamePacket(sf::Packet data)
{
}

void ServerPacketTypesManager::ReceiveEndGamePacket(sf::Packet data)
{
}
