/**
 *	SpinLock
 *
 *	@author : taekwonv@gmail.com
 */


#ifndef SPINLOCK_H
#define SPINLOCK_H

#ifdef __linux__

#include <linux/spinlock.h>

class SpinLock
{
	spinlock_t m_sl;

public:
	SpinLock(void)
	{
		spin_lock_init(&m_sl);
	}

	~SpinLock(void){}

	void lock()
	{
		spin_lock(&m_sl);
	}

	void unlock()
	{
		spin_unlock(&m_sl);
	}
};


#elif defined _WIN32 || defined _WIN64
	#if _WIN32_WINN < T0x0403
		#error for InitializeCriticalSectionAndSpinCount you must define this in project property.
	#endif

#include <windows.h>

#define SPINLOCK_SPINCOUNT	4000

class SpinLock
{
	CRITICAL_SECTION m_cs;

public:
	SpinLock(void)
	{
		InitializeCriticalSectionAndSpinCount(&m_cs, SPINLOCK_SPINCOUNT);
	}

	~SpinLock(void)
	{
		DeleteCriticalSection(&m_cs);
	}

	void lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void unlock()
	{
		LeaveCriticalSection(&m_cs);
	}
};


#else
#error "NOT SUPPORTED TARGET"
#endif

#endif
