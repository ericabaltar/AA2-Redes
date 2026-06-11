#pragma once
#include <SFML/Network.hpp>

class UdpManager
{
private:
	sf::UdpSocket socket;

	std::vector<std::pair<int, sf::Packet>> pendingCriticalPackets;
	int currentCriticalPacketId = 0;

public:
	bool Init();
	int GetNextCriticalPacketId();
	void SendCriticalPacket(int id, sf::Packet packet);
	void AttemptToSendPendingCriticalPackets();

	sf::UdpSocket& GetSocket();
};

