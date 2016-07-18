#include "distributor.h"


#include "pthread/pthread.h"
#include "ThreadPool.h"

#include "threadfunc.h"

#include "ExplodeTask.h"

//#include "utiliyapi.h"

//stl
#include <vector>

Distributor::Distributor()
{

}

Distributor::~Distributor()
{

}


bool Distributor::WMTSGetRowCol(int nLevel, double dbLeft, double dbTop, double dbRight, double dbBottom, int& nBeginRow, int& nBeginCol, int& nMaxRow, int& nMaxCol)
{
	if (!m_spCapabilities)
	{
		return false; 
	}

	// 图片索引,坐上角对应瓦片的起始行列号,  行号对应X轴，列号对应Y轴
	std::shared_ptr<TileMatrixSet> spTileMatrixSet = m_spCapabilities->getTileMatrixSet(m_sTileMatirxSet);
	if (!spTileMatrixSet)
	{
		return false;
	}

	char szID[20] = { 0 };
	_itoa_s(nLevel, szID, 10);
	std::string sId = szID;
	std::shared_ptr<TileMatrix> spTileMatrix = spTileMatrixSet->getTileMatrix(sId);

	spTileMatrix->location(dbLeft, dbTop, nBeginCol, nBeginRow);
	if (nBeginCol > 0)
		--nBeginCol;

	if (nBeginRow > 0)
		--nBeginRow;

	spTileMatrix->location(dbRight, dbBottom, nMaxCol, nMaxRow);
	if (nMaxRow < (1 << nLevel))
	{
		++nMaxRow;
	}

	if (nMaxCol < (1 << nLevel))
	{
		++nMaxCol;
	}

	return true;
}

ExplodeDistributor::ExplodeDistributor(std::vector<int>* pVec, ThreadPool* pPool, std::shared_ptr<Capabilities> spCapabilities, const std::string& sTileMatrixSet,
	double dbLeft, double  dbTop, double  dbRight, double  dbBottom)
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

ExplodeDistributor::~ExplodeDistributor()
{
	pthread_mutex_destroy(m_mutex);
	pthread_cond_destroy(m_cond);

	delete m_tThreadID;
	delete m_mutex;
	delete m_cond;
}

void ExplodeDistributor::start()
{
	pthread_create(m_tThreadID, nullptr, thread_t<ExplodeDistributor, &ExplodeDistributor::run>, this);
}

void* ExplodeDistributor::run()
{
	int nLevel;
	for (std::vector<int>::iterator it = m_pVecLevels->begin(); it != m_pVecLevels->end(); ++it)
	{
		nLevel = *it;
		int nBeginRow = 0;
		int nMaxRow = nLevel > 0 ? (1 << nLevel) : 0;
		int nBeginCol = 0;
		int nMaxCol = nLevel > 0 ? (1 << nLevel) : 0;
/*
		googleGetRowCol(false,nLevel, m_dbLeft, m_dbTop, m_dbRight, m_dbBottom,
			nBeginRow, nBeginCol, nMaxRow, nMaxCol)*/;
		
		WMTSGetRowCol(nLevel, m_dbLeft, m_dbTop, m_dbRight, m_dbBottom, nBeginRow, nBeginCol, nMaxRow, nMaxCol);

		int nRow = 0;
		int nCol = 0;
		for (nCol = nBeginCol; nCol < nMaxCol; ++nCol)
		{
			for (nRow = nBeginRow; nRow < nMaxRow; ++nRow)
			{
				ExplodeTask* pWork = new ExplodeTask(Config::Instance(), ThreadPool::getProgress());

				pWork->m_nLevel = nLevel;
				pWork->m_nRow = nRow;
				pWork->m_nCol = nCol;

				int nCount = m_pPool->addWork(pWork);
			/*	if (nCount >= m_pPool->getUpperLimit())
				{
					pthread_mutex_lock(m_mutex);
					pthread_cond_wait(m_cond, m_mutex);
				}*/
			}
		}
	}

	return nullptr;
}
