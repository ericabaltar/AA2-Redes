#include "NetworkManager.h"

void NetworkManager::Init()
{
    EstablishConnectionWithLauncherServer();

    closeServer = false;

    if(socket.bind(BIND_PORT) == sf::Socket::Status::Done)
    {
        std::cout << "Puerto bindeado correctamente." << std::endl;
    }
    else
    {
        std::cout << "Error: No se pudo bindear el puerto." << std::endl;
        closeServer = true;
    }

    //selector.add(listener);
    std::cout << "Servidor Abierto" << std::endl;
}

void NetworkManager::Update()
{
    HandleReceivedTcpPackets();

    ReceiveClientPacket();
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

void NetworkManager::ReceiveClientPacket()
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

        SPTM->ReceivePacket(packet, senderIp, senderPort);
    }
}

void NetworkManager::EstablishConnectionWithLauncherServer()
{
    disconnectFromLauncherServer = false;

    if (launcherSocket.connect(LAUNCHER_SERVER_IP, LAUNCHER_SERVER_PORT) != sf::Socket::Status::Done) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        disconnectFromLauncherServer = true;
    }
    else {
        launcherSocket.setBlocking(false);
        SPTM->SendHandshake(launcherSocket);
        HandleReceivedTcpPackets();
        std::cout << "Conectado al servidor" << std::endl;
    }
}

void NetworkManager::HandleReceivedTcpPackets()
{
    sf::Packet receivePacket;
    if (launcherSocket.receive(receivePacket) == sf::Socket::Status::Done) {
        SPTM->ReceiveTcpPacket(receivePacket);
    }
    else if (launcherSocket.receive(receivePacket) == sf::Socket::Status::Disconnected) {
        std::cout << "Servidor desconectado" << std::endl;
        disconnectFromLauncherServer = true;
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

