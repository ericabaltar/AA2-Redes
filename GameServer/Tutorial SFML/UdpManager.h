#pragma once 
#include <SFML/Network.hpp> 
#include <unordered_set> 
#include <unordered_map> 
#include "MovementPacket.h"

#define PACKET_SIZE 1024

#define NORMAL_PACKET 0b00000000 
#define CRITICAL_PACKET 0b00000001 
#define URGENT_PACKET 0b00000010

#define BIND_PORT 55000

class UdpManager {
public:
	enum class PacketType : uint8_t { MATCH_CONNECT, MATCH_START, MOVEMENT, SHOT, TAUNT, HEALTH_UPDATE, ACKNOWLEDGEMENT };

private:
	struct PendingCriticalPacket { int id; char* buffer; size_t bufferSize; sf::IpAddress ip; unsigned short port; };

	sf::UdpSocket socket;

	std::vector<PendingCriticalPacket> pendingCriticalPacketsToSend;
	std::unordered_map<std::string, std::unordered_set<int>> processedCriticalPackets;
	int currentCriticalPacketId = 0;

	std::string MakeClientKey(const sf::IpAddress& ip, unsigned short port);

	int GetNextCriticalPacketId();
	void SendCriticalPacket(const sf::IpAddress& ip, unsigned short port, int id, char* buffer, size_t bufferSize);
	void RemoveCriticalPacketFromPending(int id);
	bool PacketIsAlreadyProcessed(const std::string& key, int id);
	void ProcessedCriticalPacket(const std::string& key, const sf::IpAddress& ip, unsigned short port, int id);
	void SendAcknowledgement(const sf::IpAddress& ip, unsigned short port, int id);

	void SendData(const sf::IpAddress& ip, unsigned short port, char* buffer, size_t size);

	void ProcessPacket(PacketType type, char* buffer, size_t dataRead, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort);

	void ReceiveMovement(char* buffer, size_t dataRead, const sf::IpAddress & ip, unsigned short port);
	void ReceiveShot(char* buffer, size_t dataRead, const sf::IpAddress& ip, unsigned short port);
	void ReceiveTaunt(const sf::IpAddress& ip, unsigned short port);
	void ReceiveMatchConnect(char* buffer, size_t dataRead, const sf::IpAddress& ip, unsigned short port);

public:
	bool Init();
	void AttemptToSendPendingCriticalPackets();
	void ReceivePacket();

	void SendMatchStart(const sf::IpAddress& ip, unsigned short port);
	void SendHealthUpdate(const sf::IpAddress& ip, unsigned short port, uint8_t playerIndex, int health, int lives);
	void SendMovement(MovementPacket movement);
	void SendShot(bool towardsRight);
	void SendTaunt(const sf::IpAddress& ip, unsigned short port);
};