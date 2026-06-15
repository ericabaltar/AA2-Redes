#pragma once

#define SPAWN Spawner::Instance()
#include <queue>
#include "Object.h"

class Spawner
{
private:

	std::queue<Object*> spawnedObjects;
	Spawner() = default;
	Spawner(Spawner&) = delete;
	Spawner& operator=(const Spawner&) = delete;
public:
	inline static Spawner& Instance()
	{
		static Spawner spawner;
		return spawner;
	}

	inline void SpawnObject(Object* obj)
	{
		spawnedObjects.push(obj);
	}

	inline Object* GetSpawnedObject()
	{
		if (spawnedObjects.empty())
			return nullptr;

		Object* o = spawnedObjects.front();
		spawnedObjects.pop();
		return o;
	}

	inline int GetSpawnedObjectsCount()
	{
		return spawnedObjects.size();
	}

	inline void ClearSpawnedObjects()
	{
		while (!spawnedObjects.empty())
		{
			delete spawnedObjects.front();
			spawnedObjects.pop();
		}
	}
};

