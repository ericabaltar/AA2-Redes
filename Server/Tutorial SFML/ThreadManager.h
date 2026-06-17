#pragma once
#include <vector>
#include <thread>
#include "Task.h"
#include <mutex>
#include <deque>
#include <condition_variable>

#define ThrdM ThreadManager::Instance()

class ThreadManager
{
private:
    std::vector<std::thread> threads;
    std::deque<Task*> tasks;

    std::mutex tasksMutex;
    std::condition_variable cv;

    bool stop = false;

public:
    static ThreadManager* Instance() {
        static ThreadManager tm;
        return &tm;
    }

    ~ThreadManager() {
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            stop = true;
        }
        cv.notify_all();

        for (auto& t : threads)
            if (t.joinable())
                t.join();
    }

    void Init() {
        int count = std::thread::hardware_concurrency();

        for (int i = 0; i < count; ++i)
            threads.emplace_back(&ThreadManager::Worker, this);
    }

    void AddTask(Task* task) {
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            tasks.push_back(task);
        }
        cv.notify_one();
    }

    void AddUrgentTask(Task* task) {
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            tasks.push_front(task);
        }
        cv.notify_one();
    }

private:

    void Worker() {
        while (!stop) {
            Task* task = nullptr;

            tasksMutex.lock();

            if (tasks.empty()) {
                tasksMutex.unlock();
                return;
            }

            task = tasks.front();
            tasks.pop_front();

            tasksMutex.unlock();

            if (task != nullptr)
            {
                task->Invoke();
                delete task;
            }
        }
    }
};