#pragma once
#include <unordered_map>
#include <string>
#include "User.h"

class SharedMemory
{
private:
	std::unordered_map<std::string, int> savedInts;
	std::unordered_map<std::string, float> savedFloats;
	std::unordered_map<std::string, std::string> savedStrings;
	std::unordered_map<std::string, User> savedUsers;

public:
	void saveInt(std::string key, int value) { savedInts[key] = value; }
	void saveFloat(std::string key, float value) { savedFloats[key] = value; }
	void saveString(std::string key, std::string value) { savedStrings[key] = value; }
	void saveUser(std::string key, User value) { savedUsers[key] = value; }

	bool getInt(std::string key, int& result) {
		if (savedInts.count(key) == 0) return false;
		result = savedInts[key];
		return true;
	}
	bool getFloat(std::string key, float& result) {
		if (savedFloats.count(key) == 0) return false;
		result = savedFloats[key];
		return true;
	}
	bool getString(std::string key, std::string& result) {
		if (savedStrings.count(key) == 0) return false;
		result = savedStrings[key];
		return true;
	}
	bool getUser(std::string key, User& result) {
		if (savedUsers.count(key) == 0) return false;
		result = savedUsers[key];
		return true;
	}
};

