#include "Progress.h"
#include <iostream>


Progress::Progress()
{
	m_nCount = 1;
	m_nFailCount = 0;
	m_nNow = 0;
}

Progress::~Progress()
{

}

void Progress::setCount(int nCount)
{
	m_nCount = nCount;
}

void Progress::setNow(int nNow)
{
	m_nNow = nNow;
}

void Progress::addNow(int nAdded)
{
	m_nNow += nAdded;
}

double Progress::getProgress()
{
	double dbProgress = (double)m_nNow / (double)m_nCount;

	dbProgress *= 100.0;

	if (dbProgress > 100.0)
		return 100.0;

	return dbProgress;
}

void Progress::addFail(int nFailed)
{
	m_nFailCount += nFailed;

	m_nNow += nFailed;
}

void Progress::Show2STD()
{
	int nPersent = getProgress();
		
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

	std::cout << "=== Count£º" << m_nCount << "  === Now:" << m_nNow << "  ===Persent:" << nPersent << "  ===Failed:" << m_nFailCount;
}
