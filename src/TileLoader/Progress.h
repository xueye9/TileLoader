#ifndef PROGRESS_H__
#define PROGRESS_H__
#include <vector>

class Progress
{
public:
	Progress();
	~Progress();

	void setCount(int nCount);

	void setNow(int nNow);
	void addNow(int nAdded);

	// 获取进度百分数，单位是%，即计算值乘以了100
	double getProgress();

	void addFail(int nFailed);

	// 在标准I/O显示进度信息
	void Show2STD();

public:
	// 记录不成功的数据
	std::vector<void*> m_vecFailData;

private:
	int m_nCount;
	int m_nNow;
	int m_nFailCount;
};

#endif // PROGRESS_H__
