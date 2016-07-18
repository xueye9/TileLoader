#ifndef THREADPOOL_H__
#define THREADPOOL_H__

// std
#include <vector>
#include <queue>
#include <memory>

// pthread
#include "pthread/pthread.h"

class Work;
class Slave;
class Progress;

class ThreadPool
{
public:
	ThreadPool(int MaxThreadNum);
	~ThreadPool();

	// 添加任务
	// 返回任务总数
	int addWork(Work* pWork);

	// 激活线程
	void activate();

	// 销毁线程
	void destory();

	// 
	void waitForFinishAndDestory();

	// 判断线程池是否死掉
	bool isDead();

	// 获取最大线程数
	int getMaxThreadNumber();

	// 设置最大线程数
	void setMaxThreadNumber(int num);

	// 增加空闲线程
	void addFreeThread(Slave* p);

	// 增加工作线程
	void addBusyThread(Slave* p);

	// 
	/// @brief     doWorking
	/// @detail    ThreadPool::doWorking
	/// @param[in] int * pRemainWorkNum 正在工作的队列束
	/// @param[in] int * pFreeThreadNum 空闲线程数
	/// @return    void*
	void* doWorking(int* pRemainWorkNum, int* pFreeThreadNum);

	// 巡视任务
	void* run();

	static Progress* getProgress();

	int getPatrolInterval();

	void kill();

	int getLowerLimit() const
	{
		return m_nLowerLimit;
	}
	void setLowerLimit(int nLimit)
	{
		m_nLowerLimit = nLimit;
	}

	int getUpperLimit() const
	{
		return m_nUpperLimit;
	}
	void setUpperLimit(int nLimit)
	{
		m_nUpperLimit = nLimit;
	}

	void setCond(pthread_cond_t* cond)
	{
		m_pCond = cond;
	}

	int getRemainWork()
	{

	}
private:

	int m_nMaxThreadNum;

	// 工作线程队列锁
	pthread_mutex_t m_busyMutex;

	// 空闲线程队列锁
	pthread_mutex_t m_freeMutex;

	// 任务队列锁 
	pthread_mutex_t m_taskQueueMutex;

	// 添加任务的信号量
	pthread_cond_t m_taskQueueCond;

	std::queue<Slave*> m_queueBusySlave;
	std::queue<Slave*> m_queueFreeSlave;

	// 任务队列
	std::queue< std::shared_ptr<Work> > m_qTasks;

	bool m_bDead;

	bool m_bWaitFinish;

	static int m_nPatrolTime;

	// 线程池空闲时间
	static long long int m_llEmptyTime;

	// 前一次巡逻时发现的处于工作状态线程
	static int m_nPreWorks;

	// 监工
	pthread_t m_tOverseer;

	// 进度对象
	static Progress* m_pProgress;

	// 任务池的任务数下限，当任务数达到下线时对外发送可以添加任务的信号
	int m_nLowerLimit;

	int m_nUpperLimit;

	// 任务达到下限是对外发送的信号
	pthread_cond_t * m_pCond;
};

#endif // ThreadPool_h__

