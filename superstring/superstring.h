/**
 *	superstring
 *
 *	Provides  a set of utility to manipulate string
 *
 *	@author : taekwonv@gmail.com
 *	@modified by
 *		[date name]
 *		...
 */


#ifndef SUPERSTRING_H
#define SUPERSTRING_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>


class sstr
{
public:
	static std::string slice(const char *src, int start, int end);
	static std::wstring slice(const wchar_t *src, int start, int end);
	static std::vector<std::string> split(const char *src, const char splitter);	
	static std::vector<std::wstring> split(const wchar_t *src, const wchar_t splitter);
	static std::string join(const std::vector<std::string> &strings);
	static std::wstring join(const std::vector<std::wstring> &strings);
	static std::string format(const char *fmt, ...);
	static std::string format(const char *fmt, va_list al);
	static std::wstring format(const wchar_t *fmt, ...);	
	static std::wstring format(const wchar_t *fmt, va_list al);
	static std::string replace(const char *src, const char *find, const char *repl);
	static std::wstring replace(const wchar_t *src, const wchar_t *find, const wchar_t *repl);
	static int toInt(const char *src);
	static int toInt(const wchar_t *src);
};

#endif
