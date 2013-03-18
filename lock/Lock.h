/**
 *	AutoLock
 *
 *	@author : taekwonv@gmail.com
 */


#ifndef LOCKHEADER_H
#define LOCKHEADER_H

template <class T>
class AutoLock
{
public:
	AutoLock(T *lock) : m_lock(lock)
	{
		m_lock->lock();
	}
	~AutoLock()
	{
		m_lock->unlock();
	}

private:
	T *m_lock;
	AutoLock() {}
};

#endif