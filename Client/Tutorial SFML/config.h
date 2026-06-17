#pragma once

#define WINDOW_NAME "AA3 Shooter"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


#define PLAYER_COUNT 4

//#define PLAYER_COUNTDOWN 20.0f //In seconds

enum class SceneOption {
	NONE,
	LAUNCHER,
	LOGIN,
	LOBBY,
	WAITING_ROOM,
	GAME,
	RANKING
};

#define FONT_PATH "arial.ttf"