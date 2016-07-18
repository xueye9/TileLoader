


// c 
#include <stdio.h>  

// std
#include <iostream>  

// google glog
#include "glog/logging.h"

// windows
#include <windows.h>

// custom
#include "ThreadPool.h"
#include "Slave.h"
#include "Work.h"
#include "Progress.h"
#include "threadfunc.h"

struct locker
{
	locker(pthread_mutex_t* mutex)
	{
		pthread_mutex_lock(mutex);
		p = mutex;
	}

	~locker()
	{
		pthread_mutex_unlock(p);
	}

	pthread_mutex_t* p;
};

int ThreadPool::m_nPatrolTime = 250;
long long int ThreadPool::m_llEmptyTime = 1;
int ThreadPool::m_nPreWorks = 1;
Progress* ThreadPool::m_pProgress = new Progress;

ThreadPool::ThreadPool(int MaxThreadNum)
{
	m_nMaxThreadNum = MaxThreadNum;
	m_busyMutex = NULL;
	pthread_mutex_init(&m_busyMutex, NULL);
	pthread_mutex_init(&m_freeMutex, nullptr);
	pthread_mutex_init(&m_taskQueueMutex, nullptr);

	pthread_cond_init(&m_taskQueueCond, nullptr);

	m_bDead = false;
	m_bWaitFinish = false;

	m_nLowerLimit = -1;

	m_nUpperLimit = 1000;

	m_pCond = nullptr;
}


ThreadPool::~ThreadPool()
{
	pthread_mutex_destroy(&m_busyMutex);
	pthread_mutex_destroy(&m_freeMutex);
	pthread_mutex_destroy(&m_taskQueueMutex);

	pthread_cond_destroy(&m_taskQueueCond);

	m_busyMutex = NULL;

	destory();
}

int ThreadPool::addWork(Work* pWork)
{
	locker lc(&m_taskQueueMutex);

	while (m_qTasks.size() > getUpperLimit())
	{
		pthread_cond_wait(&m_taskQueueCond, &m_taskQueueMutex);
	}

	m_qTasks.push(std::shared_ptr<Work>(pWork));

	int nCount = m_qTasks.size();

	return nCount;
}

void ThreadPool::activate()
{
	//直接创建线程到空闲队列  
	for (int i = 0; i < m_nMaxThreadNum; ++i)
	{
		Slave* pSlave = new Slave(this);

		// 调配努力
		pSlave->Discipline();

		addBusyThread(pSlave);
	}

	// 通知监工开始监督工作
	pthread_create(&m_tOverseer, NULL, thread_t<ThreadPool, &ThreadPool::run>,this);
}

void ThreadPool::destory()
{
	//1.等待全部线程结束.  
	std::cout << "ThreadPool::Destroy begin" << std::endl;

	//1.先停止扫秒线程  
	kill();
	pthread_join(m_tOverseer, NULL);

	//1.再停止工作线程.  
	size_t size = m_queueBusySlave.size();
	while(m_queueBusySlave.size() > 0 )
	{
		class Slave* thread = m_queueBusySlave.front();
		m_queueBusySlave.pop();
		thread->kill();

		std::cout << "kill slave:" << thread->getID() << std::endl;
		delete thread;
	}

	while ( m_qTasks.size() > 0 )
	{
		m_qTasks.pop();
	}

	std::cout << "ThreadPool::Destroy end " << std::endl;
}

void ThreadPool::waitForFinishAndDestory()
{
	m_bWaitFinish = true;

	pthread_join(m_tOverseer, NULL);

	//1.再停止工作线程.  
	while(m_queueBusySlave.size() > 0)
	{
		Slave* pSlave = m_queueBusySlave.front();
		m_queueBusySlave.pop();
		pSlave->kill();
		std::cout << "thread->Destroy()" << std::endl;
		delete pSlave;
	}
}

bool ThreadPool::isDead()
{
	return m_bDead;
}

int ThreadPool::getMaxThreadNumber()
{
	locker lc0(&m_freeMutex);
	locker lc1(&m_busyMutex);

	return m_nMaxThreadNum;
}

void ThreadPool::setMaxThreadNumber(int num)
{
	// 增加线程和较少线程
	locker lc0(&m_freeMutex);
	locker lc1(&m_busyMutex);
}

void ThreadPool::addFreeThread(Slave* p)
{
	locker lc(&m_freeMutex);
	m_queueFreeSlave.push(p);
}

void ThreadPool::addBusyThread(Slave* p)
{
	locker lc(&m_busyMutex);
	m_queueBusySlave.push(p);
}

void* ThreadPool::doWorking(int* pRemainTaskNum, int* pFreeThreadNum)
{
	//1.获取空闲线程数,并且队列里还有未执行的任务.  
	//2.弹出任务.执行任务.  
	if ( !m_queueFreeSlave.empty() )
	{
		//locker lcWorkQueue(&m_taskQueueMutex);

		pthread_mutex_lock(&m_taskQueueMutex);
		if ( !m_qTasks.empty() )
		{
			std::shared_ptr<Work> task = m_qTasks.front();
			m_qTasks.pop();

			if (m_qTasks.size() < this->getLowerLimit())
			{
				pthread_cond_signal(&m_taskQueueCond);
			}
			*pRemainTaskNum = m_qTasks.size();
			pthread_mutex_unlock(&m_taskQueueMutex);

			// 1.设置任务.  
			pthread_mutex_lock(&m_freeMutex);
			Slave* restSlave = m_queueFreeSlave.front();
			m_queueFreeSlave.pop();
			*pFreeThreadNum = m_queueFreeSlave.size();
			pthread_mutex_unlock(&m_freeMutex);

			restSlave->distribute(task);

			// 2. 进入工作队列
			addBusyThread(restSlave);

			// 3.通知空闲线程启动.  
			restSlave->notify();
		}
		else
		{
			*pRemainTaskNum = 0;
			pthread_mutex_unlock(&m_taskQueueMutex);
		}
	}
	else
	{
		*pFreeThreadNum = 0;
	}

	return nullptr;
}

void* ThreadPool::run()
{
	ThreadPool* pPool = (ThreadPool*)(this);
	while (true)
	{
		if ( pPool->isDead() )
		{
			break;
		}

		int nRemainWorks = 0;
		int nFreeSalves = 0;
		pPool->doWorking(&nRemainWorks, &nFreeSalves);

		if (m_pProgress)
			m_pProgress->Show2STD();

		Sleep(m_nPatrolTime);
	}

	return NULL;
}

Progress* ThreadPool::getProgress()
{
	return m_pProgress;
}

int ThreadPool::getPatrolInterval()
{
	return m_nPatrolTime;
}

void ThreadPool::kill()
{
	m_bDead = true;
}
