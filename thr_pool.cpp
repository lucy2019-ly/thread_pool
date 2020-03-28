#include "thr_pool.h"

TaskPool::TaskPool():_run(false) {

}

TaskPool::~TaskPool() {
	stop();
}

bool TaskPool::start(int nThreads)
{
	_run = true;
	for (int i = 0; i < nThreads; ++i)
	{
		_threads.emplace_back(std::thread(&TaskPool::work, this));
	}
	return true;
}
void TaskPool::stop()
{
	{
		_run = false;
		std::unique_lock<std::mutex> ul(_mutexTasks);
		_conTasks.notify_all();
	}
	for (auto& thr : _threads)
	{
		if (thr.joinable()) {
			thr.join();
		}
	}
	auto sz = _tasks.size();
	printf("stop task limit=%zu\n",sz);
}

void TaskPool::stopWaitAll()
{
	{
		auto sz = _tasks.size();
		while (sz > 0)
		{
#ifdef _WIN32
			//Sleep(10);
#endif
		}
	}
	
}

void TaskPool::work()
{
	while (_run)
	{
		auto task = popTask();
		if (task)
		{
			task->doWork();
		}
		else 
		{
			std::unique_lock<std::mutex> ul(_mutexTasks);
			_conTasks.wait(ul);
		}
	}
}

bool TaskPool::addTask(const TaskPtr& task)
{
	std::unique_lock<std::mutex> ul(_mutexTasks);
	_tasks.push_back(task);
	_conTasks.notify_one();
	return true;
}

TaskPtr TaskPool::popTask()
{
	std::shared_ptr<Task> p;
	if (_tasks.size() > 0)
	{
		std::unique_lock<std::mutex> ul(_mutexTasks);
		p = _tasks.front();
		_tasks.pop_front();
	}
	return p;
}