#ifndef THREADFUNC_H__
#define THREADFUNC_H__

template<typename TYPE, void* (TYPE::*run)()>
void* thread_t(void* param)
{
	TYPE* This = (TYPE*)param;
	return This->run();
}

#endif // threadfunc_h__
