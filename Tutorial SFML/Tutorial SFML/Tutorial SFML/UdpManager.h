#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include "MovementPacket.h"

#define NORMAL_PACKET 0b00000000
#define CRITICAL_PACKET 0b00000001
#define URGENT_PACKET 0b00000010

class UdpManager
{
public:
	enum class PacketType : uint8_t { MOVEMENT, SHOT, TAUNT, ACKNOWLEDGEMENT };

private:
	sf::UdpSocket socket;

	std::vector<std::pair<int, sf::Packet>> pendingCriticalPacketsToSend;
	std::unordered_set<int> processedCriticalPackets;
	int currentCriticalPacketId = 0;

	int GetNextCriticalPacketId();
	void SendCriticalPacket(int id, sf::Packet packet);
	void RemoveCriticalPacketFromPending(int id);
	bool PacketIsAlreadyProcessed(int id);
	void ProcessedCriticalPacket(int id);

	void SendData(const sf::Packet& packet);

	void ReceiveMovement(sf::Packet data);
	void ReceiveShot(sf::Packet data);
	void ReceiveTaunt(sf::Packet data);

public:
	bool Init();
	void AttemptToSendPendingCriticalPackets();
	void ReceivePacket();

	void SendMovement(MovementPacket movement);
	void SendShot(bool towardsRight);
	void SendTaunt();
};

