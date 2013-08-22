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

	static bool BeginService(LPCTSTR serviceName, std::function<void(DWORD argc, LPTSTR *argv)> onRun, std::function<void()> onServiceStop);

private:
	static VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
	static VOID WINAPI ServiceCtrlHandler(DWORD);

	static std::tstring s_lastError;	
	static std::tstring	s_serviceName; //for BeginService
	static SERVICE_STATUS_HANDLE s_hSC;
	static std::function<void(DWORD argc, LPTSTR *argv)> s_onRun;
	static std::function<void()> s_onServiceStop;
};

#endif
