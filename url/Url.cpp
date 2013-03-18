#include "url.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>

using namespace std;

Url<wchar_t>::Constants<wchar_t> Url<wchar_t>::s_constants;
Url<char>::Constants<char> Url<char>::s_constants;
