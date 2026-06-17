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
	enum class PacketType : uint8_t { MATCH_CONNECT, MATCH_START, MOVEMENT, SHOT, TAUNT, HEALTH_UPDATE, ACKNOWLEDGEMENT, PING, DISCONNECT };

private:
	struct PendingCriticalPacket
	{
		int id;
		std::vector<char> buffer;

		sf::Time lastSendTime;
	};

	sf::Clock resendClock;
	const float criticalPacketCooldown = 100; //milliseconds

	sf::UdpSocket socket;

	std::vector<PendingCriticalPacket> pendingCriticalPacketsToSend;
	std::unordered_map<std::string, std::unordered_set<int>> processedCriticalPackets;
	int currentCriticalPacketId = 0;

	std::string MakeClientKey(const sf::IpAddress& ip, unsigned short port);

	int GetNextCriticalPacketId();
	void SendCriticalPacket(int id, char* buffer, size_t bufferSize);
	void RemoveCriticalPacketFromPending(int id);
	bool PacketIsAlreadyProcessed(const std::string& key, int id);
	void ProcessedCriticalPacket(const std::string& key, int id);
	void SendAcknowledgement(int id);

	void SendData(char* buffer, size_t size);

	void ProcessPacket(PacketType type, char* buffer, size_t dataRead);

	void ReceiveMovement(char* buffer, size_t dataRead);
	void ReceiveShot(char* buffer, size_t dataRead);
	void ReceiveTaunt(char* buffer, size_t dataRead);
	void ReceiveHealthUpdate(char* buffer, size_t dataRead);
	void ReceiveMatchStart(char* buffer, size_t dataRead);

	void ReceivePing(char* buffer, size_t dataRead);

	void ReceiveDisconnect(char* buffer, size_t dataRead);

public:
	bool Init();
	void AttemptToSendPendingCriticalPackets();
	void ReceivePacket();

	void SendMatchConnect(int roomId, uint8_t playerIndex);
	void SendMovement(MovementPacket movement);
	void SendShot(bool towardsRight);
	void SendTaunt();

	void SendPing();
};