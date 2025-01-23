// Copyright (c) 2020 Emmanuel Arias
#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

class Task;

class TaskQueueSubscription {
   public:
    TaskQueueSubscription(std::mutex& mutex, std::condition_variable& conditionvar)
        : m(mutex), cv(conditionvar) {}
    std::mutex& m;
    std::condition_variable& cv;
};

class TaskQueue {
   public:
    ~TaskQueue();

    // Thread safe functions
    void PushTask(Task* t);
    void PushTasks(std::vector<Task*>& tasks);
    void StopQueue();
    // Way for consumers to get the sync variables
    TaskQueueSubscription Subscribe();

    // Non-thread safe function. Consumers must ensure
    // lock acquisition
    bool HasPendingTask() const { return !m_Queue.empty(); }
    bool IsQueueStopped() const { return m_QueueIsStopped; }
    Task* GetNextTask();

   private:
    bool m_QueueIsStopped = false;
    std::queue<Task*> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_ConditionVariable;
};