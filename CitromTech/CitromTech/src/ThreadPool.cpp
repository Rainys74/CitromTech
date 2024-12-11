#include "ThreadPool.h"

namespace Citrom
{
	static void ThreadPoolWork()
	{

	}

	ThreadPool::ThreadPool(const uint32 maxThreads)
		: m_MaxThreads(maxThreads)
	{

	}

	ThreadPool::~ThreadPool()
	{
	}

	void ThreadPool::Submit(void* job, void* args)
	{
	}
}