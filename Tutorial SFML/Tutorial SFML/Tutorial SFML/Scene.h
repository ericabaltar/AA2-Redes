#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "SharedMemory.h"
#include "config.h"

class Scene
{
protected:

public:
	SceneOption nextScene = SceneOption::NONE;

	virtual void Enter(SharedMemory* _sharedMemory) {}

	virtual void Exit() {
		while (!objects.empty()) {
			Object* obj = objects.back();
			delete obj;
			objects.pop_back();
		}
	}

	virtual bool Update(sf::RenderWindow& window, float dt)
	{
		if (!window.isOpen()) return false;

		while (const std::optional event = window.pollEvent())
			HandleEvent(*event, window);

		for (Object* obj : objects) obj->update();

		Render(window);

		return true;
	}

protected:

	SharedMemory* sharedMemory;
	std::vector<Object*> objects;

	virtual void Render(sf::RenderWindow& window) {
		window.clear(sf::Color(40, 40, 40));

		for (Object* obj : objects) obj->render(window);

		//window.display();
	}

	virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& window) {
		if (event.is<sf::Event::Closed>())
			window.close();

		for (Object* obj : objects) obj->handleEvent(event);
	}
};

