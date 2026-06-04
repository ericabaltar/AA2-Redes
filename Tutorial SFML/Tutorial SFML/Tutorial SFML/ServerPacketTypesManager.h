#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdint>
#include <string>
#include "Utils.h"
#include "User.h"
#include "MapReader.h"
#include "MovementPacket.h"

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
	MOVEMENT, SHOT, TAUNT, LOBBY
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
	void ReceivePacket(sf::Packet packet);
	void SendHandshake(sf::TcpSocket& server);
	void SendLoginAttempt(std::string username, std::string password, sf::TcpSocket& server);
	void SendRegisterAttempt(std::string username, std::string password, sf::TcpSocket& server);
	void SendLobbyCreateAttempt(std::string lobbyId, sf::TcpSocket& server);
	void SendRankingPetition(int userId, sf::TcpSocket& server);
	void SendMapPetition(sf::TcpSocket& server) {}

	void SendLobbyJoinAttempt(GameMode mode, sf::UdpSocket& server);
	void SendMovement(sf::UdpSocket& server, MovementPacket movement);
	void SendShot(sf::UdpSocket& server);
	void SendTaunt(sf::UdpSocket& server);

private:
	ServerPacketTypesManager() = default;
	ServerPacketTypesManager(const ServerPacketTypesManager& ptm) = delete;
	ServerPacketTypesManager& operator=(const ServerPacketTypesManager& ptm) = delete;
	~ServerPacketTypesManager() = default;

	void SendData(sf::TcpSocket& socket, sf::Packet& packet);
	void SendUdpData(sf::UdpSocket& socket, sf::Packet& packet);

	void ReceiveLoginPacket(sf::Packet data);
	void ReceiveRegisterPacket(sf::Packet data);
	void ReceiveHandshakePacket(sf::Packet data);
	void ReceiveLobbyCreatePacket(sf::Packet data);
	void ReceiveLobbyJoinPacket(sf::Packet data);
	void ReceivePlayerCountPacket(sf::Packet data);
	void ReceiveRankingPacket(sf::Packet data);
	void ReceiveStartGamePacket(sf::Packet data);
	void ReceiveEndGamePacket(sf::Packet data);
	void ReceiveMapPacket(sf::Packet data);
};