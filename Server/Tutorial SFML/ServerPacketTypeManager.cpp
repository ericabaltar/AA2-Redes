#include "ServerPacketTypeManager.h"
#include "MatchmakingManager.h"
#include <iostream>
#include "MovementPacket.h"
#include "User.h"
#include "MovementManager.h"

sf::Packet& operator>>(sf::Packet& packet, PacketTypes& tipo) {
	int temp;
	packet >> temp;
	tipo = static_cast<PacketTypes>(temp);

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, PacketTypes& tipo) {
	int temp;
	temp = static_cast<int>(tipo);
	packet << temp;

	return packet;
}

inline sf::Packet& operator<<(sf::Packet& packet, TileType type) {
	return packet << static_cast<char>(type);
}

sf::Packet& operator>>(sf::Packet& packet, TileType& type) {
	int value;
	packet >> value;
	char charValue = static_cast<char>(value);
	type = static_cast<TileType>(charValue);
	return packet;
}

inline sf::Packet& operator<<(sf::Packet& packet, const Tile& tile) {
	return packet << tile.type << tile.x << tile.y << tile.originalChar;
}

sf::Packet& operator>>(sf::Packet& packet, Tile& tile) {
	Tile tempTile(TileType::FLOOR, 0, 0, ' ');
	packet >> tempTile.type >> tempTile.x >> tempTile.y;

	int tempValue;
	packet >> tempValue;
	char charValue = static_cast<char>(tempValue);
	tile.originalChar = charValue;
	return packet;
}

void ServerPacketTypesManager::ReceivePacket(sf::Packet packet, sf::TcpSocket& client)
{
	PacketTypes packetType;

	packet >> packetType;

	std::cout << "Recibiendo paquete del cliente, tipo: " << static_cast<int>(packetType) << std::endl;

	switch (packetType)
	{
	case PacketTypes::HANDSHAKE:
		ReceiveHandshakePacket(packet);
		break;
	case PacketTypes::LOGIN:
		ReceiveLoginPacket(packet, client);
		break;
	case PacketTypes::REGISTER:
		ReceiveRegisterPacket(packet, client);
		break;
	case PacketTypes::LOBBY_CREATE:
		ReceiveLobbyCreatePacket(packet, client);
		break;
	case PacketTypes::LOBBY_JOIN:
		ReceiveLobbyJoinPacket(packet, client);
		break;
	case PacketTypes::MOVEMENT:
		ReceiveMovementPacket(packet, client);
		break;
	case PacketTypes::RANKING:
		ReceiveRankingPacket(packet, client);
		break;
	case PacketTypes::START_GAME:
		ReceiveStartGamePacket(packet);
		break;
	case PacketTypes::END_GAME:
		ReceiveEndGamePacket(packet);
		break;
	case PacketTypes::MAP_CHECK:
		ManageMapPacket(packet, client);
		break;
	default:
		std::cout << "No se ha identificado el tipo de packete" << std::endl;
		break;
	}

	packet.clear();
}

void ServerPacketTypesManager::SendData(sf::TcpSocket& client, sf::Packet& packet)
{
	if (client.send(packet) == sf::Socket::Status::Done) {
		std::cout << "Enviando paquete..." << std::endl;
	}
	else {
		std::cerr << "Error al enviar el paquete" << std::endl;
	}
}

void ServerPacketTypesManager::SendHandshake(sf::TcpSocket& client)
{
	sf::Packet packet;
	packet << PacketTypes::HANDSHAKE << handshakeMessage;
	SendData(client, packet);
}

void ServerPacketTypesManager::SendUpdatedPlayerCount(sf::TcpSocket& client, int playerCount)
{
	sf::Packet packet;
	packet << PacketTypes::WAITING_ROOM_PLAYERS;
	packet << playerCount;

	SendData(client, packet);
}

void ServerPacketTypesManager::SendLoginResponse(sf::TcpSocket& client, bool success, const std::string& username)
{
	sf::Packet packet;
	packet << PacketTypes::LOGIN;
	packet << success;
	packet << username;

	SendData(client, packet);

	std::cout << "Respuesta de login enviada" << std::endl;
}

void ServerPacketTypesManager::SendRegisterResponse(sf::TcpSocket& client, bool success, const std::string& username)
{
	sf::Packet packet;
	packet << PacketTypes::REGISTER;
	packet << success;
	packet << username;

	SendData(client, packet);

	std::cout << "Respuesta de registro enviada" << std::endl;
}

void ServerPacketTypesManager::SendLobbyCreateResponse(sf::TcpSocket& client, bool success)
{
	sf::Packet packet;
	packet << PacketTypes::LOBBY_CREATE;
	packet << success;

	SendData(client, packet);

	std::cout << "Respuesta de creacion de lobby enviada" << std::endl;
}

void ServerPacketTypesManager::SendLobbyJoinResponse(sf::TcpSocket& client, bool success)
{
	sf::Packet packet;
	packet << PacketTypes::LOBBY_JOIN;
	packet << success;

	SendData(client, packet);

	std::cout << "Respuesta de join a lobby enviada" << std::endl;
}

void ServerPacketTypesManager::SendRankingPacket(sf::TcpSocket& client, std::vector<Database::RankingEntry>& rankings)
{
	sf::Packet packet;
	packet << PacketTypes::RANKING;

	packet << static_cast<int>(rankings.size());

	for (const auto& entry : rankings) {
		packet << entry.position;
		packet << entry.userId;
		packet << entry.username;
		packet << entry.points;

		
		std::cout << entry.position << ". " << entry.userId << " - "
			<< entry.username << " - "
			<< entry.points << " puntos" << std::endl;
	}

	SendData(client, packet);
	std::cout << "Ranking packet enviado con " << rankings.size() << " entradas." << std::endl;
}

void ServerPacketTypesManager::ReceiveHandshakePacket(sf::Packet data)
{
	std::string receiveMesage;
	data >> receiveMesage;

	std::cout << "Mensaje enviado del cliente: " << receiveMesage << std::endl;
}

void ServerPacketTypesManager::ReceiveLoginPacket(sf::Packet data, sf::TcpSocket& client)
{
	std::string loginUsername;
	std::string loginPassword;

	data >> loginUsername;
	data >> loginPassword;

	int userId = 0;

	bool correctLogin = DB->LoginUser(loginUsername, loginPassword, userId);

	SendLoginResponse(client, correctLogin, loginUsername);

	// Si es correcto, guardar tambi�n los datos del usuario (nombre y puntos del ranking)
	
	if (correctLogin) {
		//MM->AddConnectedPlayer(&client, loginUsername, 15);
	}
}

void ServerPacketTypesManager::ReceiveRegisterPacket(sf::Packet data, sf::TcpSocket& client)
{
	std::string registerUsername;
	std::string registerPassword;

	data >> registerUsername;
	data >> registerPassword;

	std::string passwordHash = bcrypt::generateHash(registerPassword);
	
	bool correctRegister = DB->RegisterUser(registerUsername, passwordHash);

	SendRegisterResponse(client, correctRegister, registerUsername);

	if (correctRegister) {
		//MM->AddConnectedPlayer(&client, registerUsername, 15);
	}
}

void ServerPacketTypesManager::ReceiveLobbyCreatePacket(sf::Packet data, sf::TcpSocket& client)
{
	std::string lobbyID;

	data >> lobbyID;

	bool successfulLobbyCreation = true; // = MM->CreateWaitingRoom(lobbyID, &client);
	 
	if (successfulLobbyCreation) {
		std::cout << "Lobby " << lobbyID << "creado exitosamente, pasando jugador a la sala de espera" << std::endl;
	}
	else {
		std::cout << "El ID " << lobbyID << " ya esta en uso" << std::endl;
	}

	SendLobbyCreateResponse(client, successfulLobbyCreation);
}

void ServerPacketTypesManager::ReceiveLobbyJoinPacket(sf::Packet data, sf::TcpSocket& client)
{
	std::string lobbyID;

	data >> lobbyID;

	std::cout << "Servidor recibe join packet" << std::endl;
	bool successfulLobbyJoin = true; // = MM->JoinWaitingRoom(lobbyID, &client);

	if (successfulLobbyJoin) {
		std::cout << "Jugador se ha unido a lobby con ID: " << lobbyID << std::endl;
	}
	else {
		std::cout << "El lobby " << lobbyID << " esta lleno o no existe" << std::endl;
	}

	SendLobbyJoinResponse(client, successfulLobbyJoin);
}

void ServerPacketTypesManager::ReceiveMovementPacket(sf::Packet data, sf::TcpSocket& client) {
	MovementPacket movement;
	data >> movement;

	User u; //from client

	MovM->AddPacket(u, movement);
}

void ServerPacketTypesManager::ReceiveRankingPacket(sf::Packet data, sf::TcpSocket& client)
{
	int playerId;

	data >> playerId;

	std::cout << "Recibida peticion de ranking para el jugador ID: " << playerId << std::endl;

	std::vector<Database::RankingEntry> topRankings = DB->GetTop10Rankings(playerId);

	SendRankingPacket(client, topRankings);
}

void ServerPacketTypesManager::ReceiveStartGamePacket(sf::Packet data)
{
}

void ServerPacketTypesManager::ReceiveEndGamePacket(sf::Packet data)
{
}

void ServerPacketTypesManager::ManageMapPacket(sf::Packet data, sf::TcpSocket& client)
{
	std::cout << "Recibida solicitud de paquete de Mapa" << std::endl;

	mapReader = new MapReader();
	mapReader->Init();

	int width, height;
	data >> width >> height;

	int tileCount = 0;
	data >> tileCount;

	std::vector<Tile> tempTileVector;

	for (int i = 0; i < tileCount; i++) {
		Tile tile;
		data >> tile;
		tempTileVector.push_back(tile);
	}

	bool isInformationValid = mapReader->CheckIfInformationIsCorrect(width, height, tempTileVector);

	sf::Packet packet;
	packet << PacketTypes::MAP_CHECK;
	if (isInformationValid) {
		std::cout << "El mapa enviado por el cliente es correcto" << std::endl;
		packet << true;
	}
	else {
		std::cout << "El mapa enviado por el cliente es incorrecto" << std::endl;
		packet << false;

		packet << mapReader->GetWidth() << mapReader->GetHeight();

		int validTileCount = 0;
		for (Tile* tile : mapReader->GetTiles()) {
			if (tile != nullptr) validTileCount++;
		}

		packet << validTileCount;

		for (Tile* tile : mapReader->GetTiles()) {
			if (tile != nullptr) {
				packet << *tile;
			}
		}
	}

	SendData(client, packet);
}
