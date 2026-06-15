#pragma once 
#include <SFML/Network.hpp> 
#include <unordered_set> 
#include <unordered_map> 
#include "MovementPacket.h"

#define PACKET_SIZE 1024

#define NORMAL_PACKET 0b00000000 
#define CRITICAL_PACKET 0b00000001 
#define URGENT_PACKET 0b00000010

class UdpManager {
public:
	enum class PacketType : uint8_t { MATCH_CONNECT, MATCH_START, MOVEMENT, SHOT, TAUNT, ACKNOWLEDGEMENT };

private:
	sf::UdpSocket socket;

	std::vector<std::pair<int, sf::Packet>> pendingCriticalPacketsToSend;
	std::unordered_map<std::string, std::unordered_set<int>> processedCriticalPackets;
	int currentCriticalPacketId = 0;

	std::string MakeClientKey(const sf::IpAddress& ip, unsigned short port);

	int GetNextCriticalPacketId();
	void SendCriticalPacket(int id, sf::Packet packet);
	void RemoveCriticalPacketFromPending(int id);
	bool PacketIsAlreadyProcessed(const std::string& key, int id);
	void ProcessedCriticalPacket(const std::string& key, int id);
	void SendAcknowledgement(int id);

	void SendData(char* buffer, size_t size);

	void ProcessPacket(PacketType type, sf::Packet data);

	void ReceiveMovement(sf::Packet data);
	void ReceiveShot(sf::Packet data);
	void ReceiveTaunt(sf::Packet data);
	void ReceiveMatchStart(sf::Packet data);

public:
	bool Init();
	void AttemptToSendPendingCriticalPackets();
	void ReceivePacket();

	void SendMatchConnect(int roomId, uint8_t playerIndex);
	void SendMovement(MovementPacket movement);
	void SendShot(bool towardsRight);
	void SendTaunt();
};