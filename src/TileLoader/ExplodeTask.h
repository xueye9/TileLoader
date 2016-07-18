#ifndef PBFWORK_H__
#define PBFWORK_H___


#include "Work.h"
#include <string>
#include "config.h"

class Progress;

class ExplodeTask : public Work
{
	enum
	{
		R_FAIL = -1,
		R_SUCCESS = 0,
	};

public:
	ExplodeTask(Config* p, Progress* progress);
	~ExplodeTask();

	virtual void* run() override;

public:
	// 瓦片等级
	int m_nLevel;

	// 起始行列号，包括此行列号的存储
	int m_nRow;
	int m_nCol;

private:
	// 资源连接串模版
	std::string m_sUrlTemplate;

	// 资源输出目录
	std::string m_sOutDir;

	Progress* m_pProgress;

};

#endif // PBFWORK_H___