#ifndef TASK_H__
#define TASK_H__
#include <string>

class Work
{
public:
	Work();
	virtual ~Work();

	virtual void* run() = 0;
};



#endif // Task_h__
