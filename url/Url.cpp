#include "url.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>


using namespace std;


template <> basic_string<char> Url<char>::encode(const string &src, bool bRemoveDoubleQuote);
template <>
basic_string<char> Url<char>::encode(const string &src, bool bRemoveDoubleQuote)
{	
    string ret;
    char ch;
    int i, hex;
	int len = src.length();
    for (i=0; i<len; i++) 
	{
        if (37 == int(src[i])) 
		{
			#ifdef __unix__
			sscanf(src.substr(i+1,2).c_str(), "%x", &hex);
			#elif defined _WIN32 || defined _WIN64
            sscanf_s(src.substr(i+1,2).c_str(), "%x", &hex);
			#endif
            ch = static_cast<char>(hex);
			if (!(bRemoveDoubleQuote && (i == 0 || i == len-3) && ch == '\"'))
				ret += ch;
            i += 2;
        } 
		else 
		{
            ret += src[i];
        }
    }
    return (ret);
}

template <> basic_string<wchar_t> Url<wchar_t>::encode(const wstring &src, bool bRemoveDoubleQuote);
template <>
basic_string<wchar_t> Url<wchar_t>::encode(const wstring &src, bool bRemoveDoubleQuote)
{	
    wstring ret;
    wchar_t ch;
    int i, hex;
	int len = src.length();
    for (i=0; i<len; i++) 
	{
        if (37 == int(src[i])) 
		{
			#ifdef __unix__
			wscanf(src.substr(i+1,2).c_str(), L"%x", &hex);
			#elif defined _WIN32 || defined _WIN64
            wscanf_s(src.substr(i+1,2).c_str(), L"%x", &hex);
			#endif
            ch = static_cast<wchar_t>(hex);
			if (!(bRemoveDoubleQuote && (i == 0 || i == len-3) && ch == L'\"'))
				ret += ch;
            i += 2;
        } 
		else 
		{
            ret += src[i];
        }
    }
    return (ret);
}