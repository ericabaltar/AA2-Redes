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
        NT->Init();

        if (NT->GetDisconnectFromServer())
            std::cout << "Disconnect from server";

        // Usuario de prueba
        opponentUser.nickname = "testOpponent";
        opponentUser.score = 0;
        opponentUser.userIndex = 999;
        opponentUser.position = 0;
        opponentUser.speed = 1.f;

        player = new PlayerCharacter();
        objects.push_back(player);

        oponent = new Character();
        objects.push_back(oponent);

        objects.push_back(new Ground({ 0.f, 450.f }, {800.f, 50.f}));
        objects.push_back(new Ground({ 400.f, 400.f }, { 50, 200.f }));
    }

    void HandleEvents(sf::RenderWindow& window)
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                    player->ResetJumpBuffer();
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