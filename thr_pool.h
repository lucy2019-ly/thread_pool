#pragma once

#include <vector>
#include <deque>
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <atomic>

class Task : public std::enable_shared_from_this<Task>
{
public:
	Task() = default;
	virtual ~Task() {

	}
	void doWork() {
		this->thr();
	}
protected:
	virtual void thr() = 0;
};

using TaskPtr = std::shared_ptr<Task>;

class TaskPool
{
public:
	TaskPool();
	~TaskPool();
	TaskPool(const TaskPool&) = delete;
	TaskPool& operator=(const TaskPool&) = delete;
	bool start(int nThreads);
	void stop();
	void stopWaitAll();
	void work();
public:
	bool addTask(const TaskPtr& task);
	TaskPtr popTask();
private:
	std::atomic_bool _run;
	std::vector<std::thread> _threads;

	std::deque<TaskPtr> _tasks;
	std::condition_variable _conTasks;
	std::mutex _mutexTasks;
};