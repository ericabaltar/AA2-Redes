#pragma once
#include <SFML/Network.hpp>

#include <string>

#define SPTM ServerPacketTypesManager::Instance()

enum PacketTypes
{
	HANDSHAKE, LOGIN, REGISTER, LOBBY_CREATE, LOBBY_JOIN, WAITING_ROOM_PLAYERS, RANKING, START_GAME, END_GAME
};

enum UdpPacketTypes
{
	MOVEMENT
};

class ServerPacketTypesManager
{
public:
	static ServerPacketTypesManager* Instance() {
		static ServerPacketTypesManager ptm;

		return &ptm;
	}

private:
	std::string handshakeMessage = "Handshake realizado";

public:
	void ReceivePacket(sf::Packet packet, sf::IpAddress& senderIp, unsigned short senderPort);
	void SendHandshake(sf::TcpSocket& client);
	void SendUpdatedPlayerCount(sf::TcpSocket& client, int playerCount);

private:
	ServerPacketTypesManager() = default;
	ServerPacketTypesManager(const ServerPacketTypesManager& ptm) = delete;
	ServerPacketTypesManager& operator=(const ServerPacketTypesManager& ptm) = delete;
	~ServerPacketTypesManager() = default;

	void SendData(sf::TcpSocket& client, sf::Packet& packet);

	void SendLoginResponse(sf::TcpSocket& client, bool success, const std::string& message);
	void SendRegisterResponse(sf::TcpSocket& client, bool success, const std::string& message);
	void SendLobbyCreateResponse(sf::TcpSocket& client, bool success);
	void SendLobbyJoinResponse(sf::TcpSocket& client, bool success);

	void ReceiveHandshakePacket(sf::Packet data);
	void ReceiveMovementPacket(sf::Packet data);
	/*
	void ReceiveLoginPacket(sf::Packet data, sf::TcpSocket& client);
	void ReceiveRegisterPacket(sf::Packet data, sf::TcpSocket& client);
	void ReceiveLobbyCreatePacket(sf::Packet data, sf::TcpSocket& client);
	void ReceiveLobbyJoinPacket(sf::Packet data, sf::TcpSocket& client);*/
	void ReceiveStartGamePacket(sf::Packet data);
	void ReceiveEndGamePacket(sf::Packet data);
};