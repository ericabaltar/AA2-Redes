#pragma once
#include <vector>
#include <thread>
#include "Task.h"
#include <mutex>
#include <deque>
#include <condition_variable>

#define ThrdM ThreadManager::Instance()

#define THREAD_COUNT std::thread::hardware_concurrency()

class ThreadManager
{
private:
    std::vector<std::thread> threads;
    std::deque<Task*> tasks;

    std::mutex tasksMutex;

    bool stop = false;

public:
    static ThreadManager* Instance() {
        static ThreadManager tm;
        return &tm;
    }

    ~ThreadManager() {
        tasksMutex.lock();
        stop = true;
        tasksMutex.unlock();

        for (std::thread& t : threads)
            if (t.joinable())
                t.join();
    }

    void Init() {
        for (int i = 0; i < THREAD_COUNT; ++i)
            threads.emplace_back(&ThreadManager::Worker, this);
    }

    void AddTask(Task* task) {
        tasksMutex.lock();
        tasks.push_back(task);
        tasksMutex.unlock();
    }

    void AddUrgentTask(Task* task) {
        std::thread t([&]() { task->Invoke(); });
        t.detach();
    }

private:

    void Worker() {
        while (!stop) {
            Task* task = new Task([]() {});

            tasksMutex.lock();
            
            if (!tasks.empty()) {
                task = tasks.front();
                tasks.pop_front();
            }

            tasksMutex.unlock();

            task->Invoke();
            delete task;
        }
    }
};