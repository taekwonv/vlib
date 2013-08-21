/**
 *	Functionality to manipulate Windows Service
 *
 *	@author : taekwonv@gmail.com
 *
 */

#ifndef MAGENT_WINSERVICE_H
#define MAGENT_WINSERVICE_H

#include <string>
#include <functional>

#ifdef _UNICODE
	#define tstring	wstring
#else
	#define tstring	string
#endif


class WinService
{
public:
	static bool CreateService(LPCTSTR command, LPCTSTR serviceName, LPCTSTR serviceDisplayName);
	static bool StartService(LPCTSTR serviceName);
	static bool StopService(LPCTSTR serviceName);	
	static bool RemoveService(LPCTSTR serviceName);
	static std::tstring GetLastError() { return s_lastError; }

	//bool BeginService(LPCTSTR serviceName, std::function<void()> onServiceStop);

private:
	static std::tstring s_lastError;
	std::function<void()>	m_onServiceStop;
	std::tstring	m_serviceName;
};

#endif
