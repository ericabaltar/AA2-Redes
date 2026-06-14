#pragma once
#include <SFML/Network.hpp>
#include <unordered_set>
#include <unordered_map>
#include "MovementPacket.h"

#define NORMAL_PACKET 0b00000000
#define CRITICAL_PACKET 0b00000001
#define URGENT_PACKET 0b00000010

#define BIND_PORT 55000

class UdpManager
{
public:
	enum class PacketType : uint8_t { MATCH_CONNECT, MOVEMENT, SHOT, TAUNT, ACKNOWLEDGEMENT };

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
	std::unordered_map<std::string, std::unordered_set<int>> processedCriticalPackets;
	int currentCriticalPacketId = 0;
	
	std::string MakeClientKey(const sf::IpAddress& ip, unsigned short port);

	int GetNextCriticalPacketId();
	void SendCriticalPacket(const sf::IpAddress& ip, unsigned short port, int id, sf::Packet packet);
	void RemoveCriticalPacketFromPending(int id);
	bool PacketIsAlreadyProcessed(const std::string& key, int id);
	void ProcessedCriticalPacket(const std::string& key, const sf::IpAddress& ip, unsigned short port, int id);
	void SendAcknowledgement(const sf::IpAddress& ip, unsigned short port, int id);

	void SendData(const sf::IpAddress& ip, unsigned short port, const sf::Packet& packet);

	void ProcessPacket(PacketType type, sf::Packet data, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort);

	void ReceiveMovement(sf::Packet data);
	void ReceiveShot(sf::Packet data);
	void ReceiveTaunt(sf::Packet data);
	void ReceiveMatchConnect(sf::Packet data, const sf::IpAddress& ip, unsigned short port);
	
public:
	bool Init();
	void AttemptToSendPendingCriticalPackets();
	void ReceivePacket();

	void SendMovement(MovementPacket movement);
	void SendShot(bool towardsRight);
	void SendTaunt();
};

