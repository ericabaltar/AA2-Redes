#include "UdpManager.h"
#include "ServerPacketTypesManager.h"
#include "NetworkManager.h"
#include <iostream>

int UdpManager::GetNextCriticalPacketId()
{
    currentCriticalPacketId++;
    return currentCriticalPacketId;
}

void UdpManager::SendCriticalPacket(int id, sf::Packet packet)
{
    pendingCriticalPacketsToSend.push_back(std::pair<int, sf::Packet>(id, packet));
    SendData(packet);
}

void UdpManager::AttemptToSendPendingCriticalPackets()
{
    for (const std::pair<int, sf::Packet>& pair : pendingCriticalPacketsToSend)
    {
        SendData(pair.second);
    }
}

void UdpManager::RemoveCriticalPacketFromPending(int id)
{
    for (std::vector<std::pair<int, sf::Packet>>::iterator it = pendingCriticalPacketsToSend.begin();
        it != pendingCriticalPacketsToSend.end(); it++)
    {
        if (it->first == id)
        {
            pendingCriticalPacketsToSend.erase(it);
            return;
        }
    }
}

bool UdpManager::CheckIfPacketIsAlreadyProcessed(int id)
{
    if (processedCriticalPackets.find(id) != processedCriticalPackets.end())
        return true;

    return false;
}

void UdpManager::SendData(const sf::Packet& packet)
{
    const void* data = packet.getData();
    std::size_t dataSize = packet.getDataSize();

    if (socket.send(data, dataSize, SERVER_IP, SERVER_PORT) == sf::Socket::Status::Done)
    {
        std::cout << "Paquete UDP enviado..." << std::endl;
    }
    else
    {
        std::cerr << "Error al enviar paquete UDP" << std::endl;
    }
}

bool UdpManager::Init()
{
    bool successful = true;

    socket.setBlocking(false);

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        std::cout << "Error al abrir socket UDP del cliente" << std::endl;
        successful = false;
    }

    return successful;
}

void UdpManager::SendMovement(MovementPacket movement)
{
    sf::Packet packet;
    uint8_t priority = NORMAL_PACKET;

    packet << priority;
    packet << UdpPacketTypes::MOVEMENT;
    packet << movement;

    SendData(packet);
}

void UdpManager::SendShot(bool towardsRight)
{
    sf::Packet packet;
    uint8_t priority = CRITICAL_PACKET;
    int id = GetNextCriticalPacketId();

    packet << priority;
    packet << id;
    packet << UdpPacketTypes::SHOT;
    packet << towardsRight;

    SendCriticalPacket(id, packet);
}

void UdpManager::SendTaunt()
{
    sf::Packet packet;
    uint8_t priority = URGENT_PACKET;

    packet << priority;
    packet << UdpPacketTypes::TAUNT;

    SendData(packet);
}

sf::UdpSocket& UdpManager::GetSocket()
{
    return socket;
}
