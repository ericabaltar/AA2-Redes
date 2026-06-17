#include "NetworkManager.h"
#include "Player.h"
#include "PingManager.h"
#include "GameRoomManager.h"

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
	while (!PingM->GetPlayersToPing().empty()) {
		Player* player = PingM->GetPlayersToPing().back();
		udp.SendPing(player->udpIp.value(), player->udpPort);
		PingM->GetPlayersToPing().pop_back();
	}
	while(!PingM->GetDisconnectedPlayers().empty()) {
		Player* player = PingM->GetDisconnectedPlayers().back();
		PingM->RemovePlayer(player);
		int playerIndex = -1;
		GameRoom* room = GRM->FindRoomByUdp(player->udpIp.value(), player->udpPort, playerIndex);
		for (int i = 0; i < room->GetPlayerAmount(); i++) {
			if (i != playerIndex) {
				Player* playerI = room->GetPlayer(i);
				PingM->RemovePlayer(playerI);
				udp.SendDisconnect(playerI->udpIp.value(), playerI->udpPort);
			}
		}
		//Delete room
		PingM->GetDisconnectedPlayers().pop_back();
	}
}

void NetworkManager::SendMovement(const sf::IpAddress& ip, unsigned short port, MovementPacket movement, bool isPlayer)
{
	udp.SendMovement(ip, port, movement, isPlayer);
}

void NetworkManager::SendShot(const sf::IpAddress& ip, unsigned short port, bool towardsRight)
{
	udp.SendShot(ip, port, towardsRight);
}

void NetworkManager::SendTaunt(const sf::IpAddress& ip, unsigned short port)
{
	udp.SendTaunt(ip, port);
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