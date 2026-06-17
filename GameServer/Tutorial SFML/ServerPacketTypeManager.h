#pragma once
#include <SFML/Network.hpp>

#include <string>

#define SPTM ServerPacketTypesManager::Instance()

#define NORMAL_PACKET 0b00000000
#define CRITICAL_PACKET 0b00000001
#define URGENT_PACKET 0b00000010

enum TcpPacketTypes
{
	HANDSHAKE, LOGIN, REGISTER, LOBBY_CREATE, LOBBY_JOIN, WAITING_ROOM_PLAYERS, MOVEMENT_TCP, RANKING, START_GAME, END_GAME, MAP_CHECK
};

enum UdpPacketTypes
{
	MOVEMENT, SHOT, TAUNT
};

class ServerPacketTypesManager
{
public:
	static ServerPacketTypesManager* Instance() {
		static ServerPacketTypesManager ptm;

		return &ptm;
	}

private:
	std::string handshakeMessage = "Handshake realizado entre servers";

public:
	void SendHandshake(sf::TcpSocket& client);
	void SendUpdatedPlayerCount(sf::TcpSocket& client, int playerCount);

	void ReceiveTcpPacket(sf::Packet packet);

private:
	ServerPacketTypesManager() = default;
	ServerPacketTypesManager(const ServerPacketTypesManager& ptm) = delete;
	ServerPacketTypesManager& operator=(const ServerPacketTypesManager& ptm) = delete;
	~ServerPacketTypesManager() = default;

	void SendData(sf::TcpSocket& client, sf::Packet& packet);

	void SendLobbyCreateResponse(sf::TcpSocket& client, bool success);
	void SendLobbyJoinResponse(sf::TcpSocket& client, bool success);

	void ReceiveHandshakePacket(sf::Packet data);

	void ReceiveMovementPacket(sf::Packet data);
	void ReceiveTauntPacket(sf::Packet data);

	void ReceiveStartGamePacket(sf::Packet data);
	void ReceiveEndGamePacket(sf::Packet data);
};