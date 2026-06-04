#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <vector>
#include "NetworkManager.h"
#include "MovementPrediction.h"
#include "Character.h"
#include "SceneManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "AA3 Shooter");

    SceneManager sceneManager = SceneManager();

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sceneManager.update(window, dt);
    }

    return 0;
}