#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "User.h"

#define SPTM ServerPacketTypesManager::Instance()

enum TcpPacketTypes
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
	std::vector<User> ranking;

public:
	void ReceivePacket(sf::Packet packet);
	void SendHandshake(sf::TcpSocket& server);
	void SendLoginAttempt(std::string username, std::string password, sf::TcpSocket& server);
	void SendRegisterAttempt(std::string username, std::string password, sf::TcpSocket& server);
	void SendLobbyCreateAttempt(std::string lobbyId, sf::TcpSocket& server);
	void SendLobbyJoinAttempt(std::string lobbyId, sf::TcpSocket& server);
	void SendRankingPetition(int userId, sf::TcpSocket& server);

	void SendUdpTest(sf::UdpSocket& server);

	inline std::vector<User> GetRanking() { return ranking; }

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
};