#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "SharedMemory.h"
#include "config.h"

class Scene
{
public:
	SceneOption nextScene = SceneOption::NONE;

	virtual void enter(SharedMemory* _sharedMemory) {}
	virtual void exit() {
		while (!objects.empty()) {
			Object* obj = objects.back();
			delete obj;
			objects.pop_back();
		}
	}

	virtual bool update(sf::RenderWindow& window) {
		if (!window.isOpen()) return false;

		while (const std::optional event = window.pollEvent())
			handleEvent(*event, window);

		for (Object* obj : objects) obj->update();

		render(window);

		return true;
	}

protected:

	SharedMemory* sharedMemory;
	std::vector<Object*> objects;

	virtual void render(sf::RenderWindow& window) {
		window.clear(sf::Color(0x000000FF));

		for (Object* obj : objects) obj->render(window);

		window.display();
	}

	virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
		if (event.is<sf::Event::Closed>())
			window.close();

		for (Object* obj : objects) obj->handleEvent(event);
	}
};

