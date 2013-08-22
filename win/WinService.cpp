#include <Windows.h>
#include <tchar.h>
#include "WinService.h"
#include "../superstring/superstring.h"

using namespace std;

tstring WinService::s_lastError;


/**                                         
 *  _ _ _ _     _____             _         
 * | | | |_|___|   __|___ ___ _ _|_|___ ___ 
 * | | | | |   |__   | -_|  _| | | |  _| -_|
 * |_____|_|_|_|_____|___|_|  \_/|_|___|___|
 *                                          
 */

bool WinService::CreateService(LPCTSTR command, LPCTSTR serviceName, LPCTSTR serviceDisplayName)
{
	SC_HANDLE hMngr, hSrv;
	if (NULL == (hMngr = OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(_T("OpenSCManager failed (%d)\n"), ::GetLastError());
		return false;
	}		
	
	DWORD dwErr = 0;
	if (NULL == (hSrv = ::CreateService(hMngr, serviceName, serviceDisplayName, GENERIC_READ, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, command, NULL, NULL, NULL, NULL, NULL)))
	{
		if (ERROR_SERVICE_EXISTS == (dwErr = ::GetLastError()))
			s_lastError = _T("Service is already exist");
		else
			s_lastError = sstr::format(_T("Cannot create a service. Error:[%d]"), dwErr);
		return false;
	}
	
	::CloseServiceHandle(hSrv);	
	return true;
}

bool WinService::StartService(LPCTSTR serviceName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = ::OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(_T("OpenSCManager failed (%d)\n"), ::GetLastError());
		return false;
	}		

	if (NULL == (hSrv = ::OpenService(hMngr, serviceName, SERVICE_START)))
	{
		s_lastError = sstr::format(_T("Cannot open the service. Error:(%d)\n"), ::GetLastError());
		return false;
	}

	if (FALSE == ::StartService(hSrv, 0, NULL))
	{
		s_lastError = sstr::format(_T("StartService failed. Error:(%d)\n"), ::GetLastError());
		return false;
	}

	::CloseServiceHandle(hSrv);
	::CloseServiceHandle(hMngr);
	return true;
}

bool WinService::StopService(LPCTSTR serviceName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = ::OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(_T("OpenSCManager failed (%d)\n"), ::GetLastError());
		return false;
	}		

	if (NULL == (hSrv = ::OpenService(hMngr, serviceName, SERVICE_STOP)))
	{
		s_lastError = sstr::format(_T("Cannot open the service. Error:(%d)\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS	status;
	if (FALSE == ::ControlService(hSrv, SERVICE_CONTROL_STOP, &status))
	{
		s_lastError = sstr::format(_T("ControlService failed. Error:(%d)\n"), ::GetLastError());
		return false;
	}

	::CloseServiceHandle(hSrv);
	::CloseServiceHandle(hMngr);
	return true;
}

bool WinService::RemoveService(LPCTSTR serviceName)
{
	SC_HANDLE hMngr, hSrv;

	if (NULL == (hMngr = ::OpenSCManager(NULL, NULL, GENERIC_WRITE)))
	{
		s_lastError = sstr::format(_T("OpenSCManager failed (%d)\n"), ::GetLastError());
		return false;
	}		

	if (NULL == (hSrv = ::OpenService(hMngr, serviceName, DELETE)))
	{
		s_lastError = sstr::format(_T("Cannot open the service. Error:(%d)\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS	status;
	::ControlService(hSrv, SERVICE_CONTROL_STOP, &status);

	if (FALSE == ::DeleteService(hSrv))
	{
		s_lastError = sstr::format(_T("DeleteService failed. Error:(%d)\n"), ::GetLastError());
		return false;
	}
	
	::CloseServiceHandle(hSrv);
	::CloseServiceHandle(hMngr);
	return true;
}


std::tstring	WinService::s_serviceName; //for BeginService
SERVICE_STATUS_HANDLE WinService::s_hSC;
std::function<void(DWORD argc, LPTSTR *argv)> WinService::s_onRun;
std::function<void()> WinService::s_onServiceStop;

VOID WINAPI WinService::ServiceCtrlHandler(DWORD ctrlCode)
{
	SERVICE_STATUS ss;
	ZeroMemory (&ss, sizeof(ss));    
	ss.dwServiceType		= SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState		= SERVICE_START_PENDING;
	ss.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ss.dwWin32ExitCode		= 0;
	ss.dwServiceSpecificExitCode	= 0;
	ss.dwCheckPoint			= 0;
	ss.dwWaitHint			= 0;

	switch (ctrlCode)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ss.dwCurrentState	= SERVICE_STOP_PENDING;
		ss.dwWaitHint		= 4000;
		SetServiceStatus(s_hSC, &ss);
		s_onServiceStop();
		break;
	}
}

VOID WINAPI WinService::ServiceMain (DWORD argc, LPTSTR *argv)
{	
	s_hSC = RegisterServiceCtrlHandler(s_serviceName.c_str(), ServiceCtrlHandler);
	if (0 == s_hSC)
	{
		OutputDebugString("s_hSC NULL");
		return;
	}

	SERVICE_STATUS ss;
	ZeroMemory (&ss, sizeof(ss));    
	ss.dwServiceType		= SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState		= SERVICE_START_PENDING;
	ss.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ss.dwWin32ExitCode		= 0;
	ss.dwServiceSpecificExitCode	= 0;
	ss.dwCheckPoint			= 0;
	ss.dwWaitHint			= 2000;

	// tell the service controller initialization
	if (FALSE == SetServiceStatus(s_hSC, &ss))
	{
		OutputDebugString("init fail");
		return;
	}

	// tell the service controller started
	ss.dwCurrentState		= SERVICE_RUNNING;
	ss.dwWin32ExitCode		= 0;
	ss.dwCheckPoint			= 0;
	if (FALSE == SetServiceStatus(s_hSC, &ss))
	{
		OutputDebugString("start fail");
		return;
	}

	s_onRun(argc, argv);

	// tell the service controller stopped
	ss.dwCurrentState		= SERVICE_STOPPED;
	ss.dwWin32ExitCode		= 0;
	ss.dwCheckPoint			= 3;
	SetServiceStatus(s_hSC, &ss);
	
	s_onServiceStop();
}

bool WinService::BeginService(LPCTSTR serviceName, std::function<void(DWORD argc, LPTSTR *argv)> onRun, std::function<void()> onServiceStop)
{
	s_serviceName	= serviceName;
	s_onRun			= onRun;
	s_onServiceStop = onServiceStop;
	
	SERVICE_TABLE_ENTRY svTable[2];
	svTable[0].lpServiceName	= (LPTSTR)s_serviceName.c_str();
	svTable[0].lpServiceProc	= (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	svTable[1].lpServiceName	= NULL;
	svTable[1].lpServiceProc	= NULL;
	return StartServiceCtrlDispatcher(svTable) ? true : false;
}

