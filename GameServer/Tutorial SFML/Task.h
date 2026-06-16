#pragma once
#include <functional>

class Task
{
private:
	std::function<void()> task;

public:
	Task(std::function<void()> t) : task(std::move(t)) {}

	void Invoke() { task(); }
};

