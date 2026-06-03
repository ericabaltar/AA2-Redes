#include <iostream>
#include <SFML/Graphics.hpp>
#include "NetworkManager.h"
//#include "SceneManager.h"

int main() {

	NT->Init();

	if (NT->GetDisconnectFromServer()) return 0;

	//sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), WINDOW_NAME);

	//SceneManager sm = SceneManager();
	bool firstTime = true;

	while (true)
	{
	if (!NT->GetDisconnectFromServer()) {
			NT->Update();

			if (firstTime) {
				NT->SendMapPetitionServerPacket();
				firstTime = false;
			}
		}

	};

	//delete window;
}