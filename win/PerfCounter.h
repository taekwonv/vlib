/**
 *	PerfCounter for Windows
 *
 *	@author : taekwonv@gmail.com
 */


#ifndef PERFCOUNTER_H
#define PERFCOUNTER_H

#include <string>
#include <pdh.h>
#include <pdhmsg.h>

class PerfCounter
{
public:
	PerfCounter(void);
	~PerfCounter(void);

	bool Add(LPCWSTR counterPath);
	bool Collect(std::wstring &data);
	std::wstring GetLastError() const { return m_lastError; }

private:
	HQUERY			m_query;
	HCOUNTER		m_counter;
	std::wstring	m_lastError;
};


#endif
