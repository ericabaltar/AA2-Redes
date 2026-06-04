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

    NT->Init();

    if (NT->GetDisconnectFromServer())
        std::cout << "No ha habido conexión con el servidor." << std::endl;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f)
            dt = 0.05f;

        sceneManager.update(window, dt);
        
        NT->Update();
    }

    return 0;
}