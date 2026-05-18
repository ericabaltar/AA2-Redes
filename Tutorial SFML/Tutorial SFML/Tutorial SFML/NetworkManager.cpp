#include "NetworkManager.h"

void NetworkManager::Init()
{
    //EstablishConnectionWithServer();
    //SPTM->SendMovement(udpServerSocket);
}

void NetworkManager::EstablishConnectionWithServer()
{
    disconnectFromServer = false;

    if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        disconnectFromServer = true;
    }
    else {
        socket.setBlocking(false);
        SPTM->SendHandshake(socket);
        HandleReceivedPackets();
        std::cout << "Conectado al servidor" << std::endl;
    }
}

void NetworkManager::Update()
{
    if (!disconnectFromServer) {
        HandleReceivedPackets();
    }
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

void NetworkManager::SendLobbyCreateAttemptPacket(std::string lobbyId)
{
    SPTM->SendLobbyCreateAttempt(lobbyId, socket);
}

void NetworkManager::SendLobbyJoinAttemptPacket(std::string lobbyId)
{
    SPTM->SendLobbyJoinAttempt(lobbyId, socket);
}

void NetworkManager::HandleReceivedPackets()
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