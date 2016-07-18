#include "Slave.h"
#include "Work.h"
#include "ThreadPool.h"


Slave::Slave(ThreadPool* pool)
{
	m_pThreadPool = pool;

	pthread_mutex_init(&m_Mutex, nullptr);
	pthread_cond_init(&m_cond, nullptr);

	m_spWork = std::shared_ptr<Work>();

	m_nID = -1;

	m_bDead = false;
}


Slave::~Slave()
{
	pthread_mutex_destroy(&m_Mutex);
	pthread_cond_destroy(&m_cond);
}

void Slave::Discipline()
{
	pthread_create(&m_thread, nullptr, &Slave::doWorking, this);
	m_nID = (int)m_thread.p;
}

void Slave::notify()
{
	pthread_cond_signal(&m_cond);
}

void Slave::wait()
{
	pthread_cond_wait(&m_cond, &m_Mutex);
}

void Slave::distribute(std::shared_ptr<Work> task)
{
	lock();
	m_spWork = task;
	unlock();
}

void Slave::rest()
{
	m_pThreadPool->addFreeThread(this);
}

bool Slave::isDead()
{
	return m_bDead;
}

int Slave::getID()
{
	return m_nID;
}

void Slave::join()
{
	int res = pthread_join(m_thread, NULL);
}

void Slave::kill()
{
	// 杀死干活的努力首先要让努力把活干完
	lock();
	m_bDead = true;
	
	notify();
	unlock();

	join();
}

void* Slave::doWorking(void* p)
{
	while (1)
	{
		Slave* pSlave = (Slave*)(p);
		
		pSlave->lock();

		if (pSlave->isDead())
		{
			pSlave->unlock();
			break;;
		}

		pSlave->unlock();

		std::shared_ptr<Work> spWork = pSlave->m_spWork;
		if (spWork)
		{
			spWork->run();
			spWork.reset(); 
		}

		// 如果需要销毁，则不进入等待
		pSlave->lock();
		if (pSlave->isDead())
		{
			pSlave->unlock();
			break;
		}

		// 加入空闲队列
		pSlave->rest();

		// 挂起
		pSlave->wait();

		pSlave->unlock();
	}
	
	return nullptr;
}

void Slave::lock()
{
	pthread_mutex_lock(&m_Mutex);
}

void Slave::unlock()
{
	pthread_mutex_unlock(&m_Mutex);
}
