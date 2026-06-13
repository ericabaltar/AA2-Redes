#include "UdpManager.h"
#include "NetworkManager.h"
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

        ProcessPacket(packetType, packet);

        packet.clear();
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

void UdpManager::ProcessPacket(PacketType type, sf::Packet data)
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
    default:
        std::cout << "No se ha identificado el tipo de paquete" << std::endl;
        break;
    }
}

void UdpManager::ReceiveMovement(sf::Packet data)
{
    MovementPacket movementPacket;
    data >> movementPacket;

    NT->SetLastValidatedMovementPacket(movementPacket);

    std::cout << "Paquete de movimiento validado recibido. ID: " << movementPacket.ID << std::endl;
}

void UdpManager::ReceiveShot(sf::Packet data)
{
}

void UdpManager::ReceiveTaunt(sf::Packet data)
{
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
    packet << PacketType::MOVEMENT;
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
    packet << PacketType::SHOT;
    packet << towardsRight;

    SendCriticalPacket(id, packet);
}

void UdpManager::SendTaunt()
{
    sf::Packet packet;
    uint8_t priority = URGENT_PACKET;

    packet << priority;
    packet << PacketType::TAUNT;

    SendData(packet);
}