#ifndef BUNDLEDISTRIBUTOR_H__
#define BUNDLEDISTRIBUTOR_H__
#include "distributor.h"
#include "ThreadPool.h"

#include "threadfunc.h"

#include "BundleTask.h"
BundleDistributor::BundleDistributor(std::vector<int>* pVec, ThreadPool* pPool, std::shared_ptr<Capabilities> spCapabilities, const std::string& sTileMatrixSet,
	double dbLeft, double dbTop, double dbRight, double dbBottom)
{
	m_tThreadID = new pthread_t;
	m_mutex = new pthread_mutex_t;
	m_cond = new pthread_cond_t;
	pthread_mutex_init(m_mutex, nullptr);
	pthread_cond_init(m_cond, nullptr);

	m_pVecLevels = pVec;
	m_pPool = pPool;
	m_dbLeft = dbLeft;
	m_dbTop = dbTop;
	m_dbRight = dbRight;
	m_dbBottom = dbBottom;

	m_pPool->setCond(m_cond);

	m_spCapabilities = spCapabilities;

	m_sTileMatirxSet = sTileMatrixSet;
}

BundleDistributor::~BundleDistributor()
{
	pthread_mutex_destroy(m_mutex);
	pthread_cond_destroy(m_cond);

	delete m_tThreadID;
	delete m_mutex;
	delete m_cond;
}

void BundleDistributor::start()
{
	pthread_create(m_tThreadID, nullptr, thread_t<ExplodeDistributor, &ExplodeDistributor::run>, this);
}

void* BundleDistributor::run()
{
	int nLevel;
	for (std::vector<int>::iterator it = m_pVecLevels->begin(); it != m_pVecLevels->end(); ++it)
	{
		nLevel = *it;
		int nBeginRow = 0;
		int nMaxRow = nLevel > 0 ? (1 << nLevel) : 0;
		int nBeginCol = 0;
		int nMaxCol = nLevel > 0 ? (1 << nLevel) : 0;

		WMTSGetRowCol(nLevel, m_dbLeft, m_dbTop, m_dbRight, m_dbBottom,
			nBeginRow, nBeginCol, nMaxRow, nMaxCol);

		int nRow = 0;
		int nCol = 0;
		int nSec = 0;

		for (nCol = nBeginCol; nCol < nMaxCol; nCol += 128)
		{
			int nSec1 = 0;
			for (nRow = nBeginRow; nRow < nMaxRow; nRow += 128)
			{
				BundleTask* pWork = new BundleTask(Config::Instance(), ThreadPool::getProgress());

				pWork->m_nLevel = nLevel;
				pWork->m_nDownLoadBeginRow = nRow;
				pWork->m_nDownLoadBeginCol = nCol;

				if (pWork->m_nDownLoadBeginRow + 128 > nMaxRow)
				{
					pWork->m_nMaxRow = nMaxRow;
				}
				else
				{
					if (0 == nSec1)
					{
						pWork->m_nMaxRow = (((nBeginRow >> 7) << 7) + 128);
					}
					else
					{
						pWork->m_nMaxRow = nRow + 128; 
					}
				}

				if (pWork->m_nDownLoadBeginCol + 128 > nMaxCol)
				{
					pWork->m_nMaxCol = nMaxCol;
				}
				else
				{
					if (0 == nSec)
					{
						pWork->m_nMaxCol = ((nBeginCol >> 7) << 7) + 128;
					}
					else
					{
						pWork->m_nMaxCol = nCol + 128;
					}
					
				}

				int nCount = m_pPool->addWork(pWork);

				if (0 == nSec1)
				{
					nCol = ((nBeginRow >> 7) << 7) + 128;
				}
				else
				{
					nCol += 128;
				}
			}

			if (0 == nSec)
			{
				nCol = ((nBeginCol >> 7) << 7) + 128;
			}
			else
			{
				nCol += 128;
			}
		}
	}	

	return NULL;

}

#endif // BUNDLEDISTRIBUTOR_H__
