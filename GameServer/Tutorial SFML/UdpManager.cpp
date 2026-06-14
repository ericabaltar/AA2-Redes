#include "UdpManager.h"
#include "ThreadManager.h"
#include "GameRoomManager.h"
#include <iostream>

sf::Packet& operator>>(sf::Packet& packet, UdpManager::PacketType& type)
{
    uint8_t temp;
    packet >> temp;
    type = static_cast<UdpManager::PacketType>(temp);

    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, UdpManager::PacketType type)
{
    uint8_t temp;
    temp = static_cast<uint8_t>(type);
    packet << temp;

    return packet;
}

int UdpManager::GetNextCriticalPacketId()
{
    currentCriticalPacketId++;
    return currentCriticalPacketId;
}

void UdpManager::SendCriticalPacket(const sf::IpAddress& ip, unsigned short port, int id, sf::Packet packet)
{
    pendingCriticalPacketsToSend.push_back({ id, packet, ip, port });
    SendData(ip, port, packet);
}

void UdpManager::RemoveCriticalPacketFromPending(int id)
{
    for (std::vector<PendingCriticalPacket>::iterator it = pendingCriticalPacketsToSend.begin();
        it != pendingCriticalPacketsToSend.end(); it++)
    {
        if (it->id == id)
        {
            pendingCriticalPacketsToSend.erase(it);
            return;
        }
    }
}

bool UdpManager::PacketIsAlreadyProcessed(int id)
{
    if (processedCriticalPackets.find(id) != processedCriticalPackets.end())
        return true;

    return false;
}

void UdpManager::ProcessedCriticalPacket(int id)
{
    processedCriticalPackets.insert(id);
}

void UdpManager::SendData(const sf::IpAddress& ip, unsigned short port, const sf::Packet& packet)
{
    const void* data = packet.getData();
    std::size_t dataSize = packet.getDataSize();

    if (socket.send(data, dataSize, ip, port) == sf::Socket::Status::Done)
    {
        std::cout << "Paquete UDP enviado..." << std::endl;
    }
    else
    {
        std::cerr << "Error al enviar paquete UDP" << std::endl;
    }
}

void UdpManager::ProcessPacket(PacketType type, sf::Packet data, std::optional<sf::IpAddress>& senderIp, unsigned short senderPort)
{
    switch (type)
    {
    case PacketType::MOVEMENT:
        ReceiveMovement(data);
        break;
    case PacketType::SHOT:
        ReceiveShot(data);
        break;
    case PacketType::TAUNT:
        ReceiveTaunt(data);
        break;
    case PacketType::MATCH_CONNECT:
        ReceiveMatchConnect(data, senderIp.value(), senderPort);
    default:
        std::cout << "No se ha identificado el tipo de paquete" << std::endl;
        break;
    }
}

void UdpManager::ReceiveMovement(sf::Packet data)
{
}

void UdpManager::ReceiveShot(sf::Packet data)
{
}

void UdpManager::ReceiveTaunt(sf::Packet data)
{
}

void UdpManager::ReceiveMatchConnect(sf::Packet data, const sf::IpAddress& ip, unsigned short port)
{
    int roomId;
    uint8_t playerIndex;

    data >> roomId;
    data >> playerIndex;

    GRM->ConnectPlayerToRoom(roomId, playerIndex, ip, port);
}

bool UdpManager::Init()
{
    bool successful = true;

    socket.setBlocking(false);

    if (socket.bind(BIND_PORT) != sf::Socket::Status::Done)
    {
        std::cout << "Error al abrir socket UDP del cliente" << std::endl;
        successful = false;
    }

    return successful;
}

void UdpManager::AttemptToSendPendingCriticalPackets()
{
    for (const PendingCriticalPacket& criticalPacket : pendingCriticalPacketsToSend)
    {
        SendData(criticalPacket.ip, criticalPacket.port, criticalPacket.packet);
    }
}

void UdpManager::ReceivePacket()
{
    char buffer[1024];
    std::size_t receivedSize;
    std::optional<sf::IpAddress> senderIp;
    unsigned short senderPort;

    if (socket.receive(buffer, sizeof(buffer), receivedSize, senderIp, senderPort) == sf::Socket::Status::Done)
    {
        std::cout << "Paquete recibido de " << senderIp.value() << ":" << senderPort << std::endl;

        sf::Packet packet;
        packet.append(buffer, receivedSize);

        uint8_t priority;
        packet >> priority;

        PacketType packetType;

        bool isCritical = (priority & CRITICAL_PACKET) != 0;
        int criticalId = 0;

        if (isCritical)
            packet >> criticalId;

        packet >> packetType;

        if (isCritical)
        {
            if (packetType == PacketType::ACKNOWLEDGEMENT)
            {
                RemoveCriticalPacketFromPending(criticalId);
            }
            else
            {
                if (PacketIsAlreadyProcessed(criticalId))
                {
                    packet.clear();
                    return;
                }
                else
                {
                    ProcessedCriticalPacket(criticalId);
                }
            }
        }

        if (priority & URGENT_PACKET)
        {
            ThrdM->AddUrgentTask(new Task([this, packetType, packet, senderIp, senderPort]() mutable {
                this->ProcessPacket(packetType, packet, senderIp, senderPort);
                }));
        }
        else
        {
            ThrdM->AddTask(new Task([this, packetType, packet, senderIp, senderPort]() mutable {
                this->ProcessPacket(packetType, packet, senderIp, senderPort);
                }));
        }

        packet.clear();
    }
}