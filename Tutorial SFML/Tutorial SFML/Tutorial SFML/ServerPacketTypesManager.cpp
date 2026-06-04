#include "ServerPacketTypesManager.h"
#include "NetworkManager.h"
//#include "LobbyManager.h"
#include "User.h"
#include "LobbyManager.h"

sf::Packet& operator>>(sf::Packet& packet, TcpPacketTypes& tipo) {
	int temp;
	packet >> temp;
	tipo = static_cast<TcpPacketTypes>(temp);

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, TcpPacketTypes& tipo) {
	int temp;
	temp = static_cast<int>(tipo);
	packet << temp;

	return packet;
}

void ServerPacketTypesManager::ReceivePacket(sf::Packet packet)
{
	TcpPacketTypes packetType;

	packet >> packetType;

	switch (packetType)
	{
	case TcpPacketTypes::HANDSHAKE:
		ReceiveHandshakePacket(packet);
		break;
	case TcpPacketTypes::LOGIN:
		ReceiveLoginPacket(packet);
		break;
	case TcpPacketTypes::REGISTER:
		ReceiveRegisterPacket(packet);
		break;
	case TcpPacketTypes::LOBBY_CREATE:
		ReceiveLobbyCreatePacket(packet);
		break;
	case TcpPacketTypes::LOBBY_JOIN:
		ReceiveLobbyJoinPacket(packet);
		break;
	case TcpPacketTypes::WAITING_ROOM_PLAYERS:
		ReceivePlayerCountPacket(packet);
		break;
	case TcpPacketTypes::RANKING:
		ReceiveRankingPacket(packet);
		break;
	case TcpPacketTypes::START_GAME:
		ReceiveStartGamePacket(packet);
		break;
	case TcpPacketTypes::END_GAME:
		ReceiveEndGamePacket(packet);
		break;
	default:
		std::cout << "No se ha identificado el tipo de paquete" << std::endl;
		break;
	}

	packet.clear();
}

void ServerPacketTypesManager::SendData(sf::TcpSocket& socket, sf::Packet& packet)
{
	if (socket.send(packet) == sf::Socket::Status::Done) {
		std::cout << "Enviando paquete..." << std::endl;
	}
	else {
		std::cerr << "Error al enviar el paquete" << std::endl;
	}
}

void ServerPacketTypesManager::SendUdpData(sf::UdpSocket& socket, sf::Packet& packet)
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

void ServerPacketTypesManager::SendHandshake(sf::TcpSocket& server)
{
	sf::Packet packet;
	packet << TcpPacketTypes::HANDSHAKE << handshakeMessage;
	SendData(server, packet);
}

void ServerPacketTypesManager::SendLoginAttempt(std::string username, std::string password, sf::TcpSocket& server)
{
	if (username.empty() || password.empty()) return;

	sf::Packet packet;
	packet << TcpPacketTypes::LOGIN;
	packet << username;
	packet << password;
	SendData(server, packet);
}

void ServerPacketTypesManager::SendRegisterAttempt(std::string username, std::string password, sf::TcpSocket& server)
{
	if (username.empty() || password.empty()) return;

	sf::Packet packet;
	packet << TcpPacketTypes::REGISTER;
	packet << username;
	packet << password;
	SendData(server, packet);
}

void ServerPacketTypesManager::SendLobbyCreateAttempt(std::string lobbyId, sf::TcpSocket& server)
{
	if (lobbyId.empty()) return;

	sf::Packet packet;

	packet << TcpPacketTypes::LOBBY_CREATE;
	packet << lobbyId;

	//LM->SetRoomId(lobbyId);

	SendData(server, packet);
}

void ServerPacketTypesManager::SendLobbyJoinAttempt(GameMode mode, sf::TcpSocket& server)
{
	sf::Packet packet;

	packet << TcpPacketTypes::LOBBY_JOIN;
	packet << (int)mode;

	//LM->SetRoomId(lobbyId);

	SendData(server, packet);
}

void ServerPacketTypesManager::SendRankingPetition(int userId, sf::TcpSocket& server)
{
	sf::Packet packet;

	packet << TcpPacketTypes::RANKING;
	packet << userId;

	SendData(server, packet);
}

void ServerPacketTypesManager::SendMovement(sf::UdpSocket& server, MovementPacket movement)
{
	sf::Packet packet;
	uint8_t priority = NORMAL_PACKET;

	packet << priority;
	packet << UdpPacketTypes::MOVEMENT;
	packet << movement;

	SendUdpData(server, packet);
}

void ServerPacketTypesManager::SendTaunt(sf::UdpSocket& server)
{
	sf::Packet packet;
	uint8_t priority = URGENT_PACKET;

	packet << priority;
	packet << UdpPacketTypes::TAUNT;

	SendUdpData(server, packet);
}


void ServerPacketTypesManager::ReceiveHandshakePacket(sf::Packet data)
{
	std::string receiveMesage;
	data >> receiveMesage;
	std::cout << "Mensaje enviado del servidor: " << receiveMesage << std::endl;
}

void ServerPacketTypesManager::ReceiveLoginPacket(sf::Packet data)
{
	bool success;
	std::string username;

	data >> success;
	data >> username;

	if (success) {
		std::cout << "Login correcto! Bienvenido " << username << std::endl;
		NT->SetSuccessfulLogin(true);
		NT->SendRankingPetitionServerPacket(16);
	}
	else {
		std::cout << "Login incorrecto" << std::endl;
	}
}

void ServerPacketTypesManager::ReceiveRegisterPacket(sf::Packet data)
{
	bool success;
	std::string username;

	data >> success;
	data >> username;

	if (success) {
		std::cout << "Registro correcto! Usuario " << username << " creado" << std::endl;
		NT->SetSuccessfulLogin(true);
		NT->SendRankingPetitionServerPacket(16);

	}
	else {
		std::cout << "Registro fallido " << std::endl;
	}
}

void ServerPacketTypesManager::ReceiveLobbyCreatePacket(sf::Packet data)
{
	bool success;

	data >> success;

	bool lobbyIDIsAvailable = success;

	if (lobbyIDIsAvailable) {
		std::cout << "Lobby creado exitosamente, pasando a la sala de espera" << std::endl;
		//LM->JoinRoom();
	}
	else {
		std::cout << "Ese ID no esta disponible ahora mismo, prueba uno diferente" << std::endl;
	}
}

void ServerPacketTypesManager::ReceiveLobbyJoinPacket(sf::Packet data)
{
	bool success;

	data >> success;

	bool lobbyIsAvailable = success;

	if (lobbyIsAvailable) {
		std::cout << "Te has unido al lobby exitosamente" << std::endl;
		//LM->JoinRoom();
	}
	else {
		std::cout << "El lobby esta lleno o no existe" << std::endl;
	}
}

void ServerPacketTypesManager::ReceivePlayerCountPacket(sf::Packet data)
{
	std::cout << "Paquete de player count recibido" << std::endl;
	int playerCount = 0;

	data >> playerCount;

	//LM->UpdatePlayerCount(playerCount);
}

void ServerPacketTypesManager::ReceiveRankingPacket(sf::Packet data)
{
	int rankingSize;
	data >> rankingSize;

	for (int i = 0; i < rankingSize; i++)
	{
		User user;
		data >> user.position;
		data >> user.userIndex;
		data >> user.nickname;
		data >> user.score;

		ranking.push_back(user);
	}

	std::cout << "\n=== TOP 10 RANKINGS ===" << std::endl;
	for (const auto& user : ranking)
	{
		std::cout << user.position << ". "
			<< user.nickname << " - "
			<< user.score << " puntos" << std::endl;
	}
}

void ServerPacketTypesManager::ReceiveStartGamePacket(sf::Packet data)
{
	int roomId = -1;
	int playerIndex = -1;

	data >> roomId;
	data >> playerIndex;

	LM->StartGame(roomId, playerIndex);
}

void ServerPacketTypesManager::ReceiveEndGamePacket(sf::Packet data)
{
}
