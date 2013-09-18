/**
 *	Url
 *
 *	Provides  a set of utility to manipulate URL as a string object
 *
 *	@author : taekwonv@gmail.com
 * 
 */

#ifndef URLCLASSLIB_H
#define URLCLASSLIB_H

#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>

template<class StringT>
struct Constants
{
	static const StringT *prodesc() { return NULL; }
	static const StringT slash() { return NULL; }
	static const StringT qsm() { return NULL; }
	static const StringT amp() { return NULL; }
};

template<>
struct Constants<wchar_t>
{
	static const wchar_t *prodesc() { return L"://"; }
	static const wchar_t slash() { return L'/'; }
	static const wchar_t qsm() { return L'/'; }
	static const wchar_t amp() { return L'&'; }
};

template<>
struct Constants<char>
{
	static const char *prodesc() { return "://"; }
	static const char slash() { return '/'; }
	static const char qsm() { return '/'; }
	static const char amp() { return '&'; }
};


template<class StringT>
class Url 
{
	Url() {}

public:
	Url(const StringT *url) : m_url(url) {	parse(url); }
	std::basic_string<StringT> url() const { return m_url; }
	std::basic_string<StringT> protocol() const { return m_protocol; }
	std::basic_string<StringT> host() const { return m_host; }
	std::basic_string<StringT> path() const { return m_path; }
	std::basic_string<StringT> query() const { return m_query; }
	std::basic_string<StringT> query(const StringT *key) const { return NULL; }
	std::basic_string<StringT> query(const StringT *key, const StringT seperator) const
	{
		std::basic_string<StringT> ret;
		const std::basic_string<StringT> sep = std::basic_string<StringT>(key) + seperator;
		typename std::basic_string<StringT>::const_iterator it = search(m_query.begin(), m_query.end(), sep.begin(), sep.end());
		if (it == m_query.end())
			return ret;
		advance(it, sep.length());
		typename std::basic_string<StringT>::const_iterator itEnd = find(it, m_query.end(), Constants<StringT>::amp());
		ret.assign(it, itEnd);
		return ret;
	}

	enum {REMOVEDOUBLEQUOTE = 1};
	static std::basic_string<StringT> encode(const std::basic_string<StringT> &src, bool bRemoveDoubleQuote = false);

private:
    void parse(const StringT *url)
	{
		const std::basic_string<StringT> url_s = url;
		const std::basic_string<StringT> prot_end(Constants<StringT>::prodesc());
		auto prot_i = search(url_s.begin(), url_s.end(), prot_end.begin(), prot_end.end());
		m_protocol.reserve(distance(url_s.begin(), prot_i));
		std::transform(url_s.begin(), prot_i, back_inserter(m_protocol), std::ptr_fun<int,int>(tolower)); // protocol is icase
		
		if (prot_i == url_s.end()) return;

		advance(prot_i, prot_end.length());
		auto path_i = find(prot_i, url_s.end(), Constants<StringT>::slash());
		m_host.reserve(distance(prot_i, path_i));
		std::transform(prot_i, path_i, back_inserter(m_host), std::ptr_fun<int,int>(tolower)); // host is icase
		auto query_i = find(path_i, url_s.end(), Constants<StringT>::qsm());
		m_path.assign(path_i, query_i);
		if (query_i != url_s.end())	++query_i;
		m_query.assign(query_i, url_s.end());
	}
    std::basic_string<StringT> m_url, m_protocol, m_host, m_path, m_query;
};

#endif
