#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include "MovementPacket.h"

#define NORMAL_PACKET 0b00000000
#define CRITICAL_PACKET 0b00000001
#define URGENT_PACKET 0b00000010

#define BIND_PORT 55000

class UdpManager
{
public:
	enum class PacketType : uint8_t { MOVEMENT, SHOT, TAUNT, ACKNOWLEDGEMENT };

private:
	struct PendingCriticalPacket
	{
		int id;
		sf::Packet packet;
		sf::IpAddress ip;
		unsigned short port;
	};

	sf::UdpSocket socket;

	std::vector<PendingCriticalPacket> pendingCriticalPacketsToSend;
	std::unordered_set<int> processedCriticalPackets;
	int currentCriticalPacketId = 0;
	
	int GetNextCriticalPacketId();
	void SendCriticalPacket(const sf::IpAddress& ip, unsigned short port, int id, sf::Packet packet);
	void RemoveCriticalPacketFromPending(int id);
	bool PacketIsAlreadyProcessed(int id);
	void ProcessedCriticalPacket(int id);

	void SendData(const sf::IpAddress& ip, unsigned short port, const sf::Packet& packet);

	void ProcessPacket(PacketType type, sf::Packet data, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort);

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

