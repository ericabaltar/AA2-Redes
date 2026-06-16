#include <SFML/Network.hpp> 
#include <SFML/System/Clock.hpp>
#include <iostream> 
#include "NetworkManager.h" 
#include "ThreadManager.h"
#include "GameRoomManager.h"

int main() {
	NT->Init();
	ThrdM->Init();

	sf::Clock clock;

	while (!NT->GetCloseServer()) {
		float dt = clock.restart().asSeconds();
		if (dt > 0.05f) dt = 0.05f;

		NT->Update();
		GRM->Update(dt);
	}

	return 0;
}