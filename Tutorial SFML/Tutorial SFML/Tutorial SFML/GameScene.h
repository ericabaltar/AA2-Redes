#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "NetworkManager.h"
#include "MovementPrediction.h"
#include "MovementReconciliation.h"
#include "Character.h"
#include "MovementInterpolation.h"
#include "PlayerCharacter.h"
#include "Bullet.h"
#include "Scene.h"
#include "Ground.h"


class GameScene : public Scene
{
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
        // Usuario de prueba
        opponentUser.nickname = "testOpponent";
        opponentUser.score = 0;
        opponentUser.userIndex = 999;
        opponentUser.position = 0;
        opponentUser.speed = 1.f;

        if(MAP->GetTiles().empty()) {
			MAP->Init();
		}

        Vector2 playerPos;
        Vector2 opponentPos;
        float tileSize = 0.f;
        for (Tile* tile : MAP->GetTiles()) {
            if (tile != nullptr) {
                switch (tile->type)
                {
                case TileType::PLAYER:
                    player = new PlayerCharacter();
					playerPos = Vector2(tile->x * MAP->GetDefaultTileSize(), tile->y * MAP->GetDefaultTileSize());
                    player->SetPosition(playerPos);
                    objects.push_back(player);
					break;

                case TileType::OPONENT:
                    oponent = new Character();
					opponentUser.position = tile->x * MAP->GetDefaultTileSize();
                    opponentPos = Vector2(tile->x * MAP->GetDefaultTileSize(), tile->y * MAP->GetDefaultTileSize());
                    oponent->SetPosition(opponentPos);
                    objects.push_back(oponent);
                    break;
				case TileType::FLOOR:
                    tileSize = MAP->GetDefaultTileSize();
                    objects.push_back(new Ground({ tile->x * MAP->GetDefaultTileSize(), tile->y * MAP->GetDefaultTileSize() }, 
                        { tileSize, tileSize}));
                    break;
                default:
                    break;
                }
            }
        }




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
        oponent->Update(dt);  // Se hace dos veces este update, no parece que cause problemas por ahora pero en el futuro podria dar
        
        Scene::Update(window, dt);

        //if (NT->GetDisconnectFromServer()) return false;

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
        //window.display();
    }
};