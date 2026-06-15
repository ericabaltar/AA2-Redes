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
	sf::Socket::Status status = mainServerSocket.receive(receivePacket);

	if (status == sf::Socket::Status::Done) {
		SPTM->ReceiveTcpPacket(receivePacket);
		receivePacket.clear();
	}
	else if (status == sf::Socket::Status::Disconnected) {
		std::cout << "Servidor desconectado" << std::endl;
		closeServer = true;
	}
}