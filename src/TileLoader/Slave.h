#ifndef THREAD_H__
#define THREAD_H__

// std
#include <memory>

//
#include "pthread/pthread.h"

/// 一个工作的线程
class Work;
class ThreadPool;
class Slave
{
public:
	Slave(ThreadPool* pool);
	~Slave();

	// 调配奴隶
	void Discipline();

	// 派发任务
	void distribute(std::shared_ptr<Work> task);

	// 干活
	void notify();

	// 休息
	void wait();

	// 加入空闲队列
	void rest();

	// 判断是否销毁
	bool isDead();

	int getID();

	void join();

	void kill();

private:
	static void* doWorking(void* p);

	std::shared_ptr<Work> m_spWork;
	ThreadPool* m_pThreadPool;

	// 
	pthread_t m_thread;



	int m_nID;

	// 互斥量
	pthread_mutex_t		m_Mutex;

	// 等待条件
	pthread_cond_t		m_cond;

	bool m_bDead;

	void lock();
	void unlock();
};

#endif // THREAD_H__