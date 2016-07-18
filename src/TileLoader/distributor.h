#ifndef DISTRIBUTOR_H__
#define DISTRIBUTOR_H__

//stl
#include <vector>
#include <string>

//pthread
#include "pthread/pthread.h"

#include "WMTS/Capabilities.h"

class ThreadPool;

class Distributor
{
public:
	Distributor();
	virtual ~Distributor();

	// 开始派发任务
	virtual void start() = 0;

	// 线程执行函数
	virtual void* run() = 0;


	/// @brief     WMTSGetRowCol
	/// @detail    Distributor::WMTSGetRowCol
	/// @param[in] int nLevel
	/// @param[in] double dbLeft
	/// @param[in] double dbTop
	/// @param[in] double dbRight
	/// @param[in] double dbBottom
	/// @param[in] int & nBeginRow
	/// @param[in] int & nBeginCol
	/// @param[in] int & nMaxRow 最大行加1，申请的时候的最大行列值是不生成请求连接的
	/// @param[in] int & nMaxCol 同上
	/// @return    bool
	bool WMTSGetRowCol(int nLevel, double dbLeft, double dbTop, double dbRight, double dbBottom,
		int& nBeginRow, int& nBeginCol, int& nMaxRow, int& nMaxCol);

	std::vector<int> *m_pVecLevels;
	double m_dbLeft;
	double m_dbTop;
	double m_dbRight;
	double m_dbBottom;
	ThreadPool *m_pPool;

	// 投影信息
	std::string m_sTileMatirxSet;

	// 元数据定义
	std::shared_ptr<Capabilities> m_spCapabilities;

protected:
	pthread_mutex_t* m_mutex;
	pthread_cond_t* m_cond;

	pthread_t* m_tThreadID;
};

// 任务分配器，根据线程池的任务上限、下限阻塞任务分配
// 下载瓦片以松散的方式存储在磁盘上的入伍分配器
class ExplodeDistributor : public Distributor
{
public:
	ExplodeDistributor(std::vector<int>* pVec, ThreadPool* pPool, std::shared_ptr<Capabilities> spCapabilities, const std::string& sTileMatrixSet,
		double dbLeft,double  dbTop,double  dbRight,double  dbBottom);

	~ExplodeDistributor();

	// 开始派发任务
	void start() override;

	// 线程执行函数
	void* run() override;
};

/// @class  BundleDistributor
/// @brief  将文件存储在arcgis压缩格式compact的文件中 
/// @author xueye
/// @date   六月 2016
class BundleDistributor : public Distributor
{
public:
	BundleDistributor(std::vector<int>* pVec, ThreadPool* pPool, std::shared_ptr<Capabilities> spCapabilities, const std::string& sTileMatrixSet,
		double dbLeft, double  dbTop, double  dbRight, double  dbBottom);

	~BundleDistributor();

	// 开始派发任务
	void start() override;

	// 线程执行函数
	void* run() override;
};





#endif // #define DISTRIBUTOR_H__

