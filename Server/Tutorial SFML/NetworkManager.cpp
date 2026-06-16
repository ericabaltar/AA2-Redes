#include "NetworkManager.h"

void NetworkManager::Init()
{
	closeServer = false;

	if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done) {
		std::cerr << "Error al iniciar el servidor" << std::endl;
		closeServer = true;
	}

	selector.add(listener);
	std::cout << "Servidor Abierto" << std::endl;
}

void NetworkManager::Update()
{
	EstablishConnectionWithClient();
	ReceiveClientPacket();
}

void NetworkManager::EstablishConnectionWithClient()
{
	if (selector.isReady(listener)) {
		newClient = new sf::TcpSocket();

		if (listener.accept(*newClient) == sf::Socket::Status::Done) {
			newClient->setBlocking(false);

			SPTM->SendHandshake(*newClient);
			selector.add(*newClient);

			//Crear aqui clase de cliente si lo acabamos poniendo

			clients.push_back(newClient);
			std::cout << "Nueva conexion establecida" << std::endl;
		}
	}
}

void NetworkManager::ReceiveClientPacket()
{
	for (int i = 0; i < clients.size(); i++) {
		if (selector.isReady(*clients[i])) {

			sf::Socket::Status status = clients[i]->receive(clientPackets[clients[i]]);

			if (status == sf::Socket::Status::Done) {
				SPTM->ReceivePacket(clientPackets[clients[i]], *clients[i]);
				clientPackets[clients[i]].clear();
			}
			else if (status == sf::Socket::Status::Disconnected) {
				selector.remove(*clients[i]);
				clientPackets.erase(clients[i]);
				delete clients[i];
				clients.erase(clients.begin() + i);
				i--;

				std::cout << "Cliente desconectado" << std::endl;
			}
		}
	}
}