#pragma once
#include <SFML/Network.hpp>

#include <string>

#define SPTM ServerPacketTypesManager::Instance()

#define NORMAL_PACKET 0b00000000
#define CRITICAL_PACKET 0b00000001
#define URGENT_PACKET 0b00000010

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
	void ReceivePacket(sf::Packet packet, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort);
	void SendHandshake(sf::TcpSocket& client);
	void SendUpdatedPlayerCount(sf::TcpSocket& client, int playerCount);

private:
	ServerPacketTypesManager() = default;
	ServerPacketTypesManager(const ServerPacketTypesManager& ptm) = delete;
	ServerPacketTypesManager& operator=(const ServerPacketTypesManager& ptm) = delete;
	~ServerPacketTypesManager() = default;

	void SendData(sf::TcpSocket& client, sf::Packet& packet);

	void SendLobbyCreateResponse(sf::TcpSocket& client, bool success);
	void SendLobbyJoinResponse(sf::TcpSocket& client, bool success);

	void ProcessPacket(sf::Packet packet);

	void ReceiveHandshakePacket(sf::Packet data);
	void ReceiveMovementPacket(sf::Packet data);

	void ReceiveStartGamePacket(sf::Packet data);
	void ReceiveEndGamePacket(sf::Packet data);
};