#pragma once
#include "Utils.h"
#include "Player.h"

class GameRoom
{
public:
	GameRoom(Player p1, Player p2, GameMode mode)
		: p1(p1), p2(p2), mode(mode) {}

private:
	Player p1, p2;
	GameMode mode;

public:
	const Player* GetPlayer(int index) const
	{
		switch (index)
		{
		case 0: return &p1;
		case 1: return &p2;
		default: return nullptr;
		}
	}

	const GameMode GetMode() const { return mode; }
};