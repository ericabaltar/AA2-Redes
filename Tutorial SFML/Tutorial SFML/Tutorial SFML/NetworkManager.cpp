#include "NetworkManager.h"

void NetworkManager::Init()
{
    EstablishConnectionWithServer();

    udpServerSocket.setBlocking(false);

    if (udpServerSocket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        std::cout << "Error al abrir socket UDP del cliente" << std::endl;
        disconnectFromServer = true;
    }
}

void NetworkManager::EstablishConnectionWithServer()
{
    disconnectFromServer = false;
    
    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);

    std::cout << "[TCP] receive status = " << (int)status << std::endl;
    if (status != sf::Socket::Status::Done) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        disconnectFromServer = true;
    }
    else {
        socket.setBlocking(false);
        SPTM->SendHandshake(socket);
        HandleReceivedTcpPackets();
        std::cout << "Conectado al servidor" << std::endl;
    }
}

void NetworkManager::HandleReceivedTcpPackets()
{
    sf::Packet receivePacket;
    if (socket.receive(receivePacket) == sf::Socket::Status::Done) {
        SPTM->ReceivePacket(receivePacket);
    }
    else if (socket.receive(receivePacket) == sf::Socket::Status::Disconnected) {
        std::cout << "Servidor desconectado" << std::endl;
        disconnectFromServer = true;
    }
}

void NetworkManager::Update()
{
    // TCP
    if (!disconnectFromServer) {
        HandleReceivedTcpPackets();
    }

    // UDP
    HandleReceivedUdpPackets();
}

sf::TcpSocket* NetworkManager::GetServerSocket()
{
    if (!disconnectFromServer)
        return &socket;

    return nullptr;
}

void NetworkManager::SendLoginAttemptServerPacket(std::string username, std::string password)
{
    SPTM->SendLoginAttempt(username, password, socket);
}

void NetworkManager::SendRegisterAttemptServerPacket(std::string username, std::string password)
{
    SPTM->SendRegisterAttempt(username, password, socket);
}

void NetworkManager::SendRankingPetitionServerPacket(int userId)
{
    SPTM->SendRankingPetition(userId, socket);
}

void NetworkManager::SendMovementPacket(MovementPacket movementPacket)
{
    SPTM->SendMovement(udpServerSocket, movementPacket);
}

void NetworkManager::SendTaunt()
{
    SPTM->SendTaunt(udpServerSocket);
}

void NetworkManager::SendLobbyCreateAttemptPacket(std::string lobbyId)
{
    SPTM->SendLobbyCreateAttempt(lobbyId, socket);
}

void NetworkManager::SendLobbyJoinAttemptPacket(GameMode mode)
{
    SPTM->SendLobbyJoinAttempt(mode, socket);
}

void NetworkManager::HandleReceivedUdpPackets()
{
    sf::Packet receivePacket;

    MovementPacket movementPacket;
    std::size_t received;
    std::optional<sf::IpAddress> senderIp;
    unsigned short senderPort;

    sf::Socket::Status udpStatus = udpServerSocket.receive(
        &movementPacket,
        sizeof(MovementPacket),
        received,
        senderIp,
        senderPort
    );

    if (udpStatus == sf::Socket::Status::Done)
    {
        if (received == sizeof(MovementPacket))
        {
            lastValidatedMovementPacket = movementPacket;
            hasValidatedMovementPacket = true;

            std::cout << "Paquete validado recibido. ID: "
                << movementPacket.ID << std::endl;
        }
    }
}

bool NetworkManager::GetLastValidatedMovementPacket(MovementPacket& packet)
{
    if (!hasValidatedMovementPacket)
        return false;

    packet = lastValidatedMovementPacket;
    hasValidatedMovementPacket = false;

    return true;
}