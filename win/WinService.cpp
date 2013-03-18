#include <Windows.h>
#include "WinService.h"
#include "../superstring/superstring.h"

using namespace std;

wstring WinService::s_lastError;


/**                                         
 *  _ _ _ _     _____             _         
 * | | | |_|___|   __|___ ___ _ _|_|___ ___ 
 * | | | | |   |__   | -_|  _| | | |  _| -_|
 * |_____|_|_|_|_____|___|_|  \_/|_|___|___|
 *                                          
 * 
 */

bool WinService::CreateService(const wstring &binPath, const wstring &cfgPath, const wstring &serviceName, const wstring &serviceDisplayName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(L"OpenSCManager failed (%d)\n", ::GetLastError());
		return false;
	}		

	DWORD dwErr = 0;
	if (NULL == (hSrv = ::CreateService(hMngr, serviceName.c_str(), serviceDisplayName.c_str(), GENERIC_READ, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, sstr::format(L"%s -c %s", binPath.c_str(), cfgPath.c_str()).c_str(), NULL, NULL, NULL, NULL, NULL)))
	{
		if (ERROR_SERVICE_EXISTS == (dwErr = ::GetLastError()))
			s_lastError = L"Service is already exist";
		else
			s_lastError = sstr::format(L"Cannot create a service. Error:[%d]", dwErr);
		return false;
	}
	
	::CloseServiceHandle(hSrv);	
	return true;
}

bool WinService::StartService(const wstring &serviceName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = ::OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(L"OpenSCManager failed (%d)\n", ::GetLastError());
		return false;
	}		

	if (NULL == (hSrv = ::OpenService(hMngr, serviceName.c_str(), SERVICE_START)))
	{
		s_lastError = sstr::format(L"Cannot open the service. Error:(%d)\n", ::GetLastError());
		return false;
	}

	if (FALSE == ::StartService(hSrv, 0, NULL))
	{
		s_lastError = sstr::format(L"StartService failed. Error:(%d)\n", ::GetLastError());
		return false;
	}

	::CloseServiceHandle(hSrv);
	::CloseServiceHandle(hMngr);
	return true;
}

bool WinService::StopService(const wstring &serviceName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = ::OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(L"OpenSCManager failed (%d)\n", ::GetLastError());
		return false;
	}		

	if (NULL == (hSrv = ::OpenService(hMngr, serviceName.c_str(), SERVICE_START)))
	{
		s_lastError = sstr::format(L"Cannot open the service. Error:(%d)\n", ::GetLastError());
		return false;
	}

	SERVICE_STATUS	status;
	if (FALSE == ::ControlService(hSrv, SERVICE_CONTROL_STOP, &status))
	{
		s_lastError = sstr::format(L"ControlService failed. Error:(%d)\n", ::GetLastError());
		return false;
	}

	::CloseServiceHandle(hSrv);
	::CloseServiceHandle(hMngr);
	return true;
}

bool WinService::RemoveService(const wstring &serviceName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = ::OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(L"OpenSCManager failed (%d)\n", ::GetLastError());
		return false;
	}		

	if (NULL == (hSrv = ::OpenService(hMngr, serviceName.c_str(), SERVICE_START)))
	{
		s_lastError = sstr::format(L"Cannot open the service. Error:(%d)\n", ::GetLastError());
		return false;
	}

	if (FALSE == ::DeleteService(hSrv))
	{
		s_lastError = sstr::format(L"DeleteService failed. Error:(%d)\n", ::GetLastError());
		return false;
	}
	
	::CloseServiceHandle(hSrv);
	::CloseServiceHandle(hMngr);
	return true;
}
