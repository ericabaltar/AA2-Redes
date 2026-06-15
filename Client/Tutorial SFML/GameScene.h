#pragma once

#include <SFML/Graphics.hpp> 
#include <iostream>

#include "NetworkManager.h" 
#include "MovementPrediction.h" 
#include "MovementReconciliation.h" 
#include "Character.h" 
#include "MovementInterpolation.h" 
#include "PlayerCharacter.h" 
#include "Bullet.h" 
#include "Scene.h" 
#include "Ground.h"
#include "MapReader.h"
#include "MatchManager.h"

class GameScene : public Scene {
private:
	MovementPrediction movementPrediction;
	MovementReconciliation movementReconciliation;
	MovementInterpolation movementInterpolation;

	PlayerCharacter* player;
	Character* oponent;

	User opponentUser;

public:
	GameScene() {}

	void Enter(SharedMemory* _sharedMemory) override
	{
		// Usuario de rival
		opponentUser.nickname = "testOpponent";
		opponentUser.score = 0;
		opponentUser.userIndex = (MM->GetPlayerIndex() == 0) ? 1 : 0;
		opponentUser.position = 0;
		opponentUser.speed = 1.f;

		if (MAP->GetTiles().empty()) {
			MAP->Init();
		}

		int myIndex = MM->GetPlayerIndex();
		TileType myTileType = (myIndex == 0) ? TileType::PLAYER : TileType::OPONENT;
		TileType oppTileType = (myIndex == 0) ? TileType::OPONENT : TileType::PLAYER;

		Vector2 playerPos;
		Vector2 opponentPos;
		float tileSize = 0.f;

		for (Tile* tile : MAP->GetTiles()) {
			if (tile != nullptr) {
				if (tile->type == myTileType) {
					player = new PlayerCharacter();
					playerPos = Vector2(tile->x * MAP->GetDefaultTileSize(), tile->y * MAP->GetDefaultTileSize());
					player->SetPosition(playerPos);
					objects.push_back(player);
				}
				else if (tile->type == oppTileType) {
					oponent = new Character();
					opponentPos = Vector2(tile->x * MAP->GetDefaultTileSize(), tile->y * MAP->GetDefaultTileSize());
					oponent->SetPosition(opponentPos);
					objects.push_back(oponent);
				}
				else if (tile->type == TileType::FLOOR) {
					tileSize = MAP->GetDefaultTileSize();
					objects.push_back(new Ground({ tile->x * MAP->GetDefaultTileSize(), tile->y * MAP->GetDefaultTileSize() },
						{ tileSize, tileSize }));
				}
			}
		}

		MM->StoreCharacters(player, oponent);

		// Configurar el paquete inicial para que el rival no aparezca en (0,0)
		MovementPacket initialPacket;
		initialPacket.ID = -1;
		initialPacket.pos = opponentPos;
		movementInterpolation.AddPacket(opponentUser, initialPacket);
	}

	virtual bool Update(sf::RenderWindow& window, float dt) override
	{
		if (player->IsDead())
		{
			std::cout << "DERROTA" << std::endl;
		}
		else if (oponent->IsDead())
		{
			std::cout << "VICTORIA" << std::endl;
		}

		movementInterpolation.Update(dt);

		Vector2 interpolatedPos = movementInterpolation.GetInterpolatedPosition(opponentUser);
		oponent->SetInterpolatedPosition(Vector2(interpolatedPos.x, interpolatedPos.y));
		oponent->Update(dt);

		Scene::Update(window, dt);

		NT->Update();

		if (movementPrediction.ShouldSendPacket(dt))
		{
			MovementPacket movementPacket = movementPrediction.CreateMovementPacket(player->GetPosition());
			movementReconciliation.AddPendingPacket(movementPacket);
			NT->SendMovementPacket(movementPacket);
		}

		MovementPacket validatedPacket;

		if (NT->GetLastValidatedMovementPacket(validatedPacket))
		{
			Vector2 correctedPosition = player->GetPosition();

			Vector2 validatedPosition(
				validatedPacket.pos.x,
				validatedPacket.pos.y
			);

			movementReconciliation.Reconcile(
				correctedPosition,
				validatedPosition,
				validatedPacket.ID
			);

			player->SetInterpolatedPosition(correctedPosition);
		}

		return true;
	}

	void Render(sf::RenderWindow& window) override
	{
		Scene::Render(window);
	}
};