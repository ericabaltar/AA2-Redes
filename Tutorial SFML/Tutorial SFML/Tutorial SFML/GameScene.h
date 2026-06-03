#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "NetworkManager.h"
#include "MovementPrediction.h"
#include "MovementInterpolation.h"
#include "PlayerCharacter.h"
#include "Scene.h"


class GameScene : Scene
{
private:
    MovementPrediction movementPrediction;
	MovementInterpolation movementInterpolation;

    sf::RectangleShape ground;

    PlayerCharacter player;
    Character oponent;

    User opponentUser;
public:
    GameScene()
        : ground({ 800.f, 50.f }) {}

    void Enter(SharedMemory* _sharedMemory) override
    {
        NT->Init();

        if (NT->GetDisconnectFromServer())
            std::cout << "Disconnect from server";

        ground.setFillColor(sf::Color(100, 100, 100));
        ground.setPosition({ 0.f, 550.f });

        // Usuario de prueba
        opponentUser.nickname = "testOpponent";
        opponentUser.score = 0;
        opponentUser.userIndex = 999;
        opponentUser.position = 0;
        opponentUser.speed = 1.f;
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
                    player.ResetJumpBuffer();
            }
        }
    }

    virtual bool Update(sf::RenderWindow& window, float dt) override
    {
        Scene::Update(window, dt);

        player.Update(dt);

		movementInterpolation.Update(dt);

        Vector2 interpolatedPos = movementInterpolation.GetInterpolatedPosition(opponentUser);
        oponent.SetPosition(sf::Vector2f(interpolatedPos.x, interpolatedPos.y));
		std::cout << "Posición interpolada del oponente: (" << interpolatedPos.x << ", " << interpolatedPos.y << ")" << std::endl;
        oponent.Update(dt);
 
        if (NT->GetDisconnectFromServer()) return false;

        NT->Update();

        if (movementPrediction.ShouldSendPacket(dt))
        {
            MovementPacket playerMovementPacket = movementPrediction.CreateMovementPacket(player.GetPosition());
            NT->SendMovementPacket(playerMovementPacket);
        }

        return true;
    }

    void Render(sf::RenderWindow& window) override
    {
        Scene::Render(window);

        window.draw(ground);
        window.draw(player.GetSprite());
        window.draw(oponent.GetSprite());

        window.display();
    }
};