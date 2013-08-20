#include "superstring.h"


/**
 *  _____                 _____ _       _         
 * |   __|_ _ ___ ___ ___|   __| |_ ___|_|___ ___ 
 * |__   | | | . | -_|  _|__   |  _|  _| |   | . |
 * |_____|___|  _|___|_| |_____|_| |_| |_|_|_|_  |
 *           |_|                             |___|
 */

using namespace std;

template<class T>
basic_string<T> slice_impl(const T *src, int start, int end, T endc)
{
	basic_string<T> ret = src + start;
	ret[end-start] = endc;
	return ret;
}

string sstr::slice(const char *src, int start, int end)
{
	return slice_impl<char>(src, start, end, '\0');
}

wstring sstr::slice(const wchar_t *src, int start, int end)
{
	return slice_impl<wchar_t>(src, start, end, L'\0');
}



template<class T, class SplitterT>
vector<basic_string<T>> split_impl(const T*src, const T splitter, const T* (*schr)(const T*, SplitterT), T endc)
{
	vector<basic_string<T>> ret;
	int start = 0, end = 0;
	const T* p = src;

	while ((p = schr(p, splitter)) != NULL)
	{
		end = (int)(p - src);
		ret.push_back(slice_impl<T>(src, start, end, endc));
		start = end + 1;
		p++;
	}	
	ret.push_back((T*)(src + start));
	return ret;
}

vector<string> sstr::split(const char *src, char splitter)
{
	return split_impl<char, int>(src, splitter, &strchr, '\0');
}

vector<wstring> sstr::split(const wchar_t *src, const wchar_t splitter)
{
	return split_impl<wchar_t, wchar_t>(src, splitter, &wcschr, L'\0');
}



template<class T>
basic_string<T> join_impl(const vector<basic_string<T>> &strings)
{
	basic_string<T> ret;
	for (auto it = strings.begin(); it != strings.end(); ++it)
		ret = ret + it->c_str();
	return ret;
}

string sstr::join(const vector<string> &strings)
{
	return join_impl<char>(strings);
}

wstring sstr::join(const vector<wstring> &strings)
{
	return join_impl<wchar_t>(strings);
}



template<class T>
basic_string<T> format_impl(int (*func_snprintf)(T*, size_t, const T*, va_list), const T *fmt, va_list ap) 
{
	int size = 1024;
	basic_string<T> str;
	
	while (1) 
	{
		str.resize(size);
		int n = func_snprintf((T *)str.c_str(), size, fmt, ap);
		va_end(ap);
		if (n > -1 && n < size) 
		{
			str.resize(n);
			return str;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;

		va_start(ap, fmt);
	}
	return str;
}

string sstr::format(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	return format_impl(vsnprintf, fmt, ap);
}

string sstr::format(const char *fmt, va_list al)
{		
	return format_impl(vsnprintf, fmt, al);
}

wstring sstr::format(const wchar_t *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	return format_impl(_vsnwprintf, fmt, ap);	
}

wstring sstr::format(const wchar_t *fmt, va_list al)
{	
	return format_impl(_vsnwprintf, fmt, al);
}

template<class T>
basic_string<T> replace_impl(size_t (*func_strlen)(const T*), const T *src, const T *find, const T *repl) 
{
	basic_string<T> str(src);
	size_t sfrom = func_strlen(find);
	
	unsigned pos = 0;
	do
	{
		pos = str.find(find, pos);
		if (pos == string::npos)
			break;
		str.replace(pos, sfrom, repl);
	}while (1);	
	return str;
}

string sstr::replace(const char *src, const char *find, const char *repl)
{
	return replace_impl<char>(strlen, src, find, repl);
}

wstring sstr::replace(const wchar_t *src, const wchar_t *find, const wchar_t *repl)
{
	return replace_impl<wchar_t>(wcslen, src, find, repl);
}



int sstr::toInt(const char *src)
{
	return atoi(src);
}

int sstr::toInt(const wchar_t *src)
{
	return _wtoi(src);
}
