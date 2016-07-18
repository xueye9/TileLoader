#ifndef GENERALWORK_H__
#define GENERALWORK_H__


#include "Work.h"
#include <string>
#include "config.h"

class Progress;

class BundleTask : public Work
{
	enum
	{
		R_FAIL = -1,
		R_SUCCESS = 0,
	};

public:
	BundleTask(Config* p, Progress* progress);
	~BundleTask();

	virtual void* run() override;

public:
	// 瓦片等级
	int m_nLevel;

	// 起始行列号，包括此行列号的存储
	int m_nDownLoadBeginRow;
	int m_nDownLoadBeginCol;

	// 下载小于最大行列的瓦片，不下载最大行列
	int m_nMaxRow;
	int m_nMaxCol;

private:
	// 资源连接串模版
	std::string m_sUrlTemplate;

	// 资源输出目录
	std::string m_sOutDir;

	Progress* m_pProgress;

};

#endif // generalwork_h__