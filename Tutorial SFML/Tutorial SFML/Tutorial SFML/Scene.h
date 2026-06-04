#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "SharedMemory.h"
#include "config.h"
#include "Spawner.h"

class Scene
{
private:
	void DestroyObjectsPendingDestroy()
	{
		for (int i = objects.size() - 1; i >= 0; i--)
		{
			if (objects[i]->IsPendingDestroy())
			{
				delete objects[i];
				objects.erase(objects.begin() + i);
			}
		}
	}

	void SpawnObjectsPendingSpawn()
	{
		while (SPAWN.GetSpawnedObjectsCount() > 0)
		{
			objects.push_back(SPAWN.GetSpawnedObject());
		}
	}

	void CheckCollisions()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			for (int j = i + 1; j < objects.size(); j++)
			{
				if (objects[i]->CheckCollision(objects[j]->GetCollider()))
				{
					objects[i]->OnCollisionEnter(objects[j]);
					objects[j]->OnCollisionEnter(objects[i]);
				}
			}
		}
	}
	
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

		DestroyObjectsPendingDestroy();
		SpawnObjectsPendingSpawn();

		for (Object* obj : objects) obj->Update(dt);

		CheckCollisions();

		Render(window);

		return true;
	}

protected:

	SharedMemory* sharedMemory;
	std::vector<Object*> objects;

	virtual void Render(sf::RenderWindow& window) {
		window.clear(sf::Color(40, 40, 40));

		for (Object* obj : objects) obj->Render(window);

		window.display();
	}

	virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& window) {
		if (event.is<sf::Event::Closed>())
			window.close();

		for (Object* obj : objects) obj->HandleEvent(event);
	}
};

