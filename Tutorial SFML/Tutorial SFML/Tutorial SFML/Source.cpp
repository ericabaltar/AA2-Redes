#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <vector>
#include "NetworkManager.h"
#include "MovementPrediction.h"
#include "Character.h"

int main()
{
    MovementPrediction movementPrediction;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML Test");

    NT->Init();

    if (NT->GetDisconnectFromServer()) return 0;

    sf::RectangleShape ground({ 800.f, 50.f });
    ground.setFillColor(sf::Color(100, 100, 100));
    ground.setPosition({ 0.f, 550.f });

    sf::Clock clock;

    Character player;

    while (window.isOpen())
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

        float dt = clock.restart().asSeconds();
        float groundY = ground.getPosition().y;

        player.DetectInput();
        player.Update(dt, groundY);

        window.clear(sf::Color::Black);
        window.draw(ground);
        window.draw(player.GetSprite());
        window.display();

        if (NT->GetDisconnectFromServer()) continue;
            
        NT->Update();

        if (movementPrediction.ShouldSendPacket(dt))
        {
            MovementPacket movementPacket = movementPrediction.CreateMovementPacket(player.GetPosition());
            NT->SendMovementPacket(movementPacket);
        }
    }

    return 0;
}