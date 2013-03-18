/**
 *	Functionality to manipulate Windows Service
 *
 *	@author : taekwonv@gmail.com
 *	@modified by
 *		[date name]
 *		...
 */


#ifndef MAGENT_WINSERVICE_H
#define MAGENT_WINSERVICE_H

#include <string>


class WinService
{
public:
	static bool CreateService(const std::wstring &binPath, const std::wstring &cfgPath, \
								const std::wstring &serviceName, const std::wstring &serviceDisplayName);
	static bool StartService(const std::wstring &serviceName);
	static bool StopService(const std::wstring &serviceName);	
	static bool RemoveService(const std::wstring &serviceName);
	static std::wstring GetLastError() { return s_lastError; }

private:
	static std::wstring s_lastError;
};

#endif
