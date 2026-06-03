#pragma once
#include <vector>
#include <thread>
#include "Task.h"
#include <mutex>
#include <deque>

#define THREAD_COUNT 100
#define ThrdM ThreadManager::Instance()

class ThreadManager
{
private:
	std::vector<std::thread*> threads;
	std::deque<Task*> tasks;
	std::mutex tasksMutex;

public:
	static ThreadManager* Instance() {
		static ThreadManager tm;

		return &tm;
	}

	void Init() {
		for (int i = 0; i < THREAD_COUNT; ++i) threads.emplace_back(Worker);
	}

	void AddTask(Task* task) {
		tasksMutex.lock();
		tasks.emplace_back(task);
		tasksMutex.unlock();
	}

	void AddUrgentTask(Task* task) {
		tasksMutex.lock();
		tasks.emplace_front(task);
		tasksMutex.unlock();
	}

private:

	void Worker() {
		bool closeThread = false;

		while (!closeThread) {

			Task* task;

			tasksMutex.lock();
			if (!tasks.empty()) {
				task = tasks.front();
				tasks.pop_front();
			}
			else {
				closeThread = true;
			}
			tasksMutex.unlock();

			if(task) task->Invoke();
		}
	}
};

