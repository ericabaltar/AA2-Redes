#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include "MovementPacket.h"

#define NORMAL_PACKET 0b00000000
#define CRITICAL_PACKET 0b00000001
#define URGENT_PACKET 0b00000010

class UdpManager
{
private:
	sf::UdpSocket socket;

	std::vector<std::pair<int, sf::Packet>> pendingCriticalPacketsToSend;
	std::unordered_set<int> processedCriticalPackets;
	int currentCriticalPacketId = 0;

	int GetNextCriticalPacketId();
	void SendCriticalPacket(int id, sf::Packet packet);
	void RemoveCriticalPacketFromPending(int id);
	bool CheckIfPacketIsAlreadyProcessed(int id);

	void SendData(const sf::Packet& packet);

public:
	bool Init();
	void AttemptToSendPendingCriticalPackets();

	void SendMovement(MovementPacket movement);
	void SendShot(bool towardsRight);
	void SendTaunt();

	sf::UdpSocket& GetSocket();
};

