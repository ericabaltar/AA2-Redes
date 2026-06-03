//#include <SFML/Graphics.hpp>
//#include <optional>
//#include <cmath>
//#include <vector>
//#include "GameScene.h"
//#include "NetworkManager.h"
//#include "MovementPrediction.h"
//#include "Character.h"
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "AA3 Shooter");
//
//    GameScene gameScene;
//    gameScene.Enter(nullptr);
//
//    sf::Clock clock;
//
//    while (window.isOpen())
//    {
//        float dt = clock.restart().asSeconds();
//
//        gameScene.HandleEvents(window);
//        gameScene.Update(window, dt);
//    }
//
//    return 0;
//}