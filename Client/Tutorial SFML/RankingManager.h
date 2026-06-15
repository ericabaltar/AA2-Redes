#pragma once
#include <vector>
#include "User.h"

#define RM RankingManager::Instance()

class RankingManager
{
private:
	std::vector<User> ranking;

public:
	static RankingManager* Instance() {
		static RankingManager rm;

		return &rm;
	}

	inline std::vector<User> GetRanking() { return ranking; }
	inline void SetRanking(std::vector<User> _ranking) { ranking = _ranking; }
};

