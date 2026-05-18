#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <vector>
#include "GameScene.h"
#include "NetworkManager.h"
#include "MovementPrediction.h"
#include "Character.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "AA3 Shooter");

    GameScene gameScene;

    if (!gameScene.Init())
        return 0;

    while (window.isOpen())
    {
        gameScene.HandleEvents(window);
        gameScene.Update();
        gameScene.Render(window);
    }

    return 0;
}