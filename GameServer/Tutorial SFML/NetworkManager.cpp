#include "NetworkManager.h"

void NetworkManager::Init()
{
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

    //EstablishConnectionWithClient();
    ReceiveClientPacket();
    //CheckForDisconnection();
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

