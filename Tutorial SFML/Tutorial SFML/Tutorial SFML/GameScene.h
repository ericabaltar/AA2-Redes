#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "NetworkManager.h"
#include "MovementPrediction.h"
#include "PlayerCharacter.h"

class GameScene
{
private:
    MovementPrediction movementPrediction;

    sf::RectangleShape ground;
    sf::Clock clock;

    PlayerCharacter player;
    Character oponent;
public:
    GameScene()
        : ground({ 800.f, 50.f })
    {
        ground.setFillColor(sf::Color(100, 100, 100));
        ground.setPosition({ 0.f, 550.f });
    }

    bool Init()
    {
        NT->Init();

        if (NT->GetDisconnectFromServer())
            return false;

        return true;
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

    void Update()
    {
        float dt = clock.restart().asSeconds();
        float groundY = ground.getPosition().y;

        player.Update(dt, groundY);
        oponent.Update(dt, groundY);

        if (NT->GetDisconnectFromServer()) return;

        NT->Update();

        if (movementPrediction.ShouldSendPacket(dt))
        {
            MovementPacket playerMovementPacket = movementPrediction.CreateMovementPacket(player.GetPosition());
            NT->SendMovementPacket(playerMovementPacket);
        }
    }

    void Render(sf::RenderWindow& window)
    {
        window.clear(sf::Color(40, 40, 40));

        window.draw(ground);
        window.draw(player.GetSprite());
        window.draw(oponent.GetSprite());

        window.display();
    }
};