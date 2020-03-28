#include "thr_pool.h"

class PrintTask : public Task
{
public:
	PrintTask(int aa):_aa(aa) {
		//printf(" PrintTask aa=%d\n", _aa);
	}
	~PrintTask() {
		printf("~PrintTask aa=%d\n", _aa);
	}
	virtual void thr()
	{
		printf("this aa=%d,thr_id=%d\n", _aa,std::this_thread::get_id());
	}
private:
	int _aa;
};

void testThreadPool()
{
	TaskPool taskPool;
	taskPool.start(4);
	auto task1 = std::make_shared<PrintTask>(11);
	taskPool.addTask(task1);
	taskPool.addTask(std::make_shared<PrintTask>(22));
	taskPool.addTask(std::make_shared<PrintTask>(33));
	taskPool.addTask(std::make_shared<PrintTask>(44));
	taskPool.addTask(std::make_shared<PrintTask>(55));
	taskPool.addTask(std::make_shared<PrintTask>(66));
}
