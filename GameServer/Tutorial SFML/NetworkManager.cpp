#include "NetworkManager.h"

void NetworkManager::Init()
{
    EstablishConnectionWithLauncherServer();
    
    if (!udp.Init())
        closeServer = true;
}

void NetworkManager::Update()
{
    HandleReceivedTcpPackets();

    udp.ReceivePacket();
    udp.AttemptToSendPendingCriticalPackets();
}
/*
void NetworkManager::EstablishConnectionWithClient()
{
    if (selector.isReady(listener)) {
        newClient = new sf::TcpSocket();

        if (listener.accept(*newClient) == sf::Socket::Status::Done) {
            newClient->setBlocking(false);
            
            SPTM->SendHandshake(*newClient);
            selector.add(*newClient);

            //Se crearia aqui el cliente con su clase Cliente

            clients.push_back(newClient);
            std::cout << "Nueva conexion establecida" << std::endl;
        }
    }
}*/

void NetworkManager::EstablishConnectionWithLauncherServer()
{
    closeServer = false;

    if (mainServerSocket.connect(MAIN_SERVER_IP, MAIN_SERVER_PORT) != sf::Socket::Status::Done) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        closeServer = true;
    }
    else {
        mainServerSocket.setBlocking(false);
        SPTM->SendHandshake(mainServerSocket);
        HandleReceivedTcpPackets();
        std::cout << "Conectado al servidor" << std::endl;
    }
}

void NetworkManager::HandleReceivedTcpPackets()
{
    sf::Packet receivePacket;
    if (mainServerSocket.receive(receivePacket) == sf::Socket::Status::Done) {
        SPTM->ReceiveTcpPacket(receivePacket);
    }
    else if (mainServerSocket.receive(receivePacket) == sf::Socket::Status::Disconnected) {
        std::cout << "Servidor desconectado" << std::endl;
        closeServer = true;
    }
}

/*
void NetworkManager::CheckForDisconnection()
{
    if (!selector.isReady(listener)) {
        for (int i = 0; i < clients.size(); i++) {
            if (selector.isReady(*clients[i])) {
                sf::Packet packet;

                if (clients[i]->receive(packet) == sf::Socket::Status::Disconnected) {
                    selector.remove(*clients[i]);
                    delete clients[i];
                    clients.erase(clients.begin() + i);
                    i--;

                    std::cout << "Cliente desconectado" << std::endl;
                }
            }
        }
    }
}*/

