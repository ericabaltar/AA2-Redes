#pragma once 
#include <vector>
#include <iostream>
#include <mutex>
#include "Player.h" 
#include "Utils.h"
#include "MapReader.h"
#include "AABB.h"
#include "MovementPacket.h"
#include "NetworkManager.h"
#include "PingManager.h"
#include "MovementManager.h"

struct HeadlessBullet {
	Vector2 position;
	Vector2 velocity;
	AABB collider;
	int ownerIndex;
	float lifeTime;

	HeadlessBullet() : collider(Vector2(0.f, 0.f), Vector2(16.f, 16.f)) {}
};

class GameRoom {
public:
	GameRoom(int roomId, GameMode mode) : id(roomId), gameMode(mode) {
		MAP->Init();
	}

private:
	int id;
	GameMode gameMode;
	static const int maxPlayers = 2;
	Player players[maxPlayers];
	int playersAdded = 0;
	bool gameStarted = false;

	std::vector<HeadlessBullet> bullets;
	std::mutex roomMutex;

public:
	int GetId() { return id; }

	GameMode GetGameMode()
	{
		return gameMode;
	}

	bool AddPlayer(Player& player)
	{
		std::lock_guard<std::mutex> lock(roomMutex);
		if (IsFull())
			return false;

		players[playersAdded] = player;
		playersAdded++;

		return true;
	}

	Player* GetPlayer(int index)
	{
		if (index >= 0 && index < playersAdded)
			return &players[index];

		return nullptr;
	}

	int GetPlayerAmount()
	{
		return playersAdded;
	}

	bool IsFull()
	{
		return playersAdded == maxPlayers;
	}

	bool CanStartGame()
	{
		return IsFull() && !gameStarted;
	}

	void StartGame()
	{
		std::lock_guard<std::mutex> lock(roomMutex);
		if (CanStartGame()) {
			gameStarted = true;
			for (Player player : players) PingM->AddPlayer(&player);
		}
	}

	bool HasStarted()
	{
		return gameStarted;
	}

	void Update(float dt)
	{
		std::lock_guard<std::mutex> lock(roomMutex);
		if (!gameStarted) return;

		for (int i = (int)bullets.size() - 1; i >= 0; i--)
		{
			bullets[i].position = bullets[i].position + bullets[i].velocity * dt;
			bullets[i].collider.SetTopLeft(bullets[i].position - Vector2(8.f, 8.f));
			bullets[i].lifeTime -= dt;

			bool destroyed = (bullets[i].lifeTime <= 0.f);

			if (!destroyed)
			{
				for (Tile* t : MAP->GetTilesByType(TileType::FLOOR))
				{
					AABB wall(Vector2(t->x * MAP->GetDefaultTileSize(), t->y * MAP->GetDefaultTileSize()),
						Vector2(MAP->GetDefaultTileSize(), MAP->GetDefaultTileSize()));

					if (bullets[i].collider.CheckOverlappingAABB(&wall))
					{
						destroyed = true;
						break;
					}
				}
			}

			if (!destroyed)
			{
				for (int p = 0; p < playersAdded; p++)
				{
					if (bullets[i].ownerIndex != players[p].index)
					{
						if (bullets[i].collider.CheckOverlappingAABB(&players[p].collider))
						{
							players[p].health--;
							std::cout << "[Gameplay] Jugador " << players[p].index << " fue alcanzado. HP: " << players[p].health << std::endl;

							if (players[p].health <= 0)
							{
								players[p].lives--;
								players[p].health = 5;
								std::cout << "[Gameplay] Jugador " << players[p].index << " perdio una vida. Vidas: " << players[p].lives << std::endl;
								if (players[p].lives <= 0) {
									std::cout << "[Gameplay] Jugador " << players[p].index << " ELIMINADO." << std::endl;
								}
							}

							// ENVIAMOS EL UPDATE DE VIDA A TODOS LOS CLIENTES DE LA SALA
							for (int j = 0; j < playersAdded; ++j) {
								Player* notifyPlayer = &players[j];
								NT->GetUdpManager()->SendHealthUpdate(
									notifyPlayer->udpIp.value(),
									notifyPlayer->udpPort,
									players[p].index,
									players[p].health,
									players[p].lives
								);
							}

							destroyed = true;
							break;
						}
					}
				}
			}

			if (destroyed)
			{
				bullets.erase(bullets.begin() + i);
			}
		}
	}

	void HandleMovement(int playerIndex, MovementPacket movement)
	{
		roomMutex.lock();

		MovementPacket lastValidMovement = MovM->ValidatePacket(&players[playerIndex], movement);

		for (int i = 0; i < maxPlayers; i++) 
			NT->SendMovement(players[i].udpIp.value(), players[i].udpPort, movement, i == playerIndex);

		roomMutex.unlock();
	}

	void HandleShot(int playerIndex, bool facingRight)
	{
		std::lock_guard<std::mutex> lock(roomMutex);
		if (playerIndex >= 0 && playerIndex < playersAdded)
		{
			HeadlessBullet b;
			float offset = facingRight ? 40.f : -40.f;
			b.position = Vector2(players[playerIndex].position.x + offset, players[playerIndex].position.y);
			b.velocity = Vector2(facingRight ? 700.f : -700.f, 0.f);
			b.collider.SetTopLeft(b.position - Vector2(8.f, 8.f));
			b.ownerIndex = players[playerIndex].index;
			b.lifeTime = 5.f;

			bullets.push_back(b);

			int otherPlayerIndex = playerIndex == 0 ? 1 : 0;
			Player* otherPlayer = GetPlayer(otherPlayerIndex);
			NT->SendShot(otherPlayer->udpIp.value(), otherPlayer->udpPort, facingRight);
		}
	}

	void HandleTaunt(int playerIndex)
	{
		std::lock_guard<std::mutex> lock(roomMutex);
		if (playerIndex >= 0 && playerIndex < playersAdded)
		{
			int otherPlayerIndex = playerIndex == 0 ? 1 : 0;
			Player* otherPlayer = GetPlayer(otherPlayerIndex);

			NT->SendTaunt(otherPlayer->udpIp.value(), otherPlayer->udpPort);
		}
	}
};