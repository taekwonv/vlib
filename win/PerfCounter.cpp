#include "PerfCounter.h"
#include "../superstring/superstring.h"

#pragma comment(lib, "pdh.lib")

using namespace std;


/**                                                 
 *  _____         ___ _____             _           
 * |  _  |___ ___|  _|     |___ _ _ ___| |_ ___ ___ 
 * |   __| -_|  _|  _|   --| . | | |   |  _| -_|  _|
 * |__|  |___|_| |_| |_____|___|___|_|_|_| |___|_|  
 *                                                  
 */


PerfCounter::PerfCounter(void) : m_query(NULL)
{
	PDH_STATUS Status;
	Status = PdhOpenQuery(NULL, NULL, &m_query);
	if (Status != ERROR_SUCCESS) 
		m_lastError = sstr::format(L"PdhOpenQuery failed with status 0x%x.", Status);
}

PerfCounter::~PerfCounter(void)
{
	if (m_query) 
		PdhCloseQuery(m_query);
}

bool PerfCounter::Add(LPCWSTR counterPath)
{
	if (!m_query)
	{
		m_lastError = L"Not initilized";
        return false;
	}
		
	PDH_STATUS Status = PdhAddCounter(m_query, counterPath, 0, &m_counter);
    if (Status != ERROR_SUCCESS) 
    {
		m_lastError = sstr::format(L"\nPdhAddCounter failed with status 0x%x.", Status);
        return false;
    }

	//
    // Most counters require two sample values to display a formatted value.
    // PDH stores the current sample value and the previously collected
    // sample value. This call retrieves the first value that will be used
    // by PdhGetFormattedCounterValue in the first iteration of the loop
    // Note that this value is lost if the counter does not require two
    // values to compute a displayable value.
    //
	
	wstring temp;
	Collect(temp);
	return true;
}

bool PerfCounter::Collect(wstring &data)
{
	if (!m_query)
	{
		m_lastError = L"Not initilized";
        return false;
	}

	PDH_STATUS Status = PdhCollectQueryData(m_query);
    if (Status != ERROR_SUCCESS) 
    {
		m_lastError = sstr::format(L"\nPdhCollectQueryData failed with 0x%x.\n", Status);
        return false;
    }

	//
	// Compute a displayable value for the counter.
	//

	DWORD CounterType;
	PDH_FMT_COUNTERVALUE DisplayValue;
	Status = PdhGetFormattedCounterValue(m_counter,
		PDH_FMT_DOUBLE,
		&CounterType,
		&DisplayValue);
	if (Status != ERROR_SUCCESS) 
	{
		wprintf(L"PdhGetFormattedCounterValue failed with status 0x%x.", Status);
		return false;
	}

	data = sstr::format(L"%.20g", DisplayValue.doubleValue);

	return true;
}
