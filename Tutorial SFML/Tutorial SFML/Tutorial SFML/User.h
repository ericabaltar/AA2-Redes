#pragma once
#include <string>
struct User
{
	std::string nickname;
	int score;
	int userIndex;
	int position;
	float speed;

	bool operator<(const User& other) const
	{
		return userIndex < other.userIndex;
	}

	//// (Opcional) igualdad útil si más tarde se usa unordered_map
	//bool operator==(const User& other) const
	//{
	//	return userIndex == other.userIndex;
	//}
};