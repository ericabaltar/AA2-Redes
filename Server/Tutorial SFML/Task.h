#pragma once
#include <functional>
class Task
{
private:
	std::function<void()> task;

public:
	void Invoke() { task(); }
};

