#include "UdpManager.h"
#include "ServerPacketTypesManager.h"
#include <iostream>

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

int UdpManager::GetNextCriticalPacketId()
{
    currentCriticalPacketId++;
    return currentCriticalPacketId;
}

void UdpManager::SendCriticalPacket(int id, sf::Packet packet)
{
    pendingCriticalPackets.push_back(std::pair<int, sf::Packet>(id, packet));
    SPTM->SendCriticalPacket(socket, packet);
}

void UdpManager::AttemptToSendPendingCriticalPackets()
{
    for (const std::pair<int, sf::Packet>& pair : pendingCriticalPackets)
    {
        SPTM->SendCriticalPacket(socket, pair.second);
    }
}

sf::UdpSocket& UdpManager::GetSocket()
{
    return socket;
}
