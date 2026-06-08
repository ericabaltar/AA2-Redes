#pragma once
#include <unordered_map>
#include "Scene.h"
#include "LauncherScene.h"
#include "GameScene.h"
#include "LoginScene.h"
#include "LobbyWaitingScene.h"
#include "MatchmakingScene.h"
#include "RankingScene.h"
#include <string>
#include "User.h"
#include "config.h"
#include "SharedMemory.h"

class SceneManager
{
	std::unordered_map<SceneOption, Scene*> scenes;
	Scene* curScene;
	SharedMemory* sharedMemory;

public:
	SceneManager() {
		//TODO: Receive all external info from server
		User user = { "eauna", 69420, 0 };

		User otherUsers[PLAYER_COUNT - 1] = {
			{"blah", 0, 1},
			{"bweh", 0, 2},
			{"blip", 0, 3}
		};

		sharedMemory = new SharedMemory();
		sharedMemory->saveUser("user", user);
		for (int i = 0; i < PLAYER_COUNT; ++i) {
			if (i == user.userIndex) continue;

			for (int j = 0; j < PLAYER_COUNT - 1; ++j)
				if (i == otherUsers[j].userIndex)
					sharedMemory->saveUser("user" + i, otherUsers[j]);
		}

		scenes[SceneOption::LAUNCHER] = new LauncherScene();
		scenes[SceneOption::GAME] = new GameScene();
		scenes[SceneOption::LOGIN] = new LoginScene();
		scenes[SceneOption::LOBBY] = new MatchmakingScene();
		scenes[SceneOption::WAITING_ROOM] = new LobbyWaitingScene();
		scenes[SceneOption::RANKING] = new RankingScene();
		curScene = scenes[SceneOption::GAME];
		curScene->Enter(sharedMemory);
	}

	bool update(sf::RenderWindow& window, float dt) {
		if (curScene->nextScene != SceneOption::NONE) {
			curScene->Exit();
			curScene = scenes[curScene->nextScene];
			curScene->Enter(sharedMemory);
		}

		return curScene->Update(window, dt);
	}
};

