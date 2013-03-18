#include "http.h"

using namespace http;
using namespace std;

#ifdef __linux__

...

#elif defined _WIN32 || defined _WIN64



/**
 * _____ _ _         _   _____                     _   
 * |     | |_|___ ___| |_| __  |___ ___ _ _ ___ ___| |_ 
 * |   --| | | -_|   |  _|    -| -_| . | | | -_|_ -|  _|
 * |_____|_|_|___|_|_|_| |__|__|___|_  |___|___|___|_|  
 *                                   |_|                
 */

template <class StringT, class Func>
std::unique_ptr<ClientRequest<StringT>> request(RequestOption, Func callback)
{
	unique_ptr<ClientRequest<StringT>> req = new ClientRequest<StringT>;
	

	return req;
}

template <class StringT>
void ClientRequest<StringT>::write(char *chunk, EncodingType)
{
}

template <class StringT>
void ClientRequest<StringT>::end(char *chunk, EncodingType)
{
}

template <class StringT>
void ClientRequest<StringT>::abort()
{
}

template <class StringT>
template <class Func>
void ClientRequest<StringT>::setTimeout(int, Func callback)
{
}




                                                         
/**
 *  _____ _ _         _   _____                             
 * |     | |_|___ ___| |_| __  |___ ___ ___ ___ ___ ___ ___ 
 * |   --| | | -_|   |  _|    -| -_|_ -| . | . |   |_ -| -_|
 * |_____|_|_|___|_|_|_| |__|__|___|___|  _|___|_|_|___|___|
 *                                     |_|                  
 * 
 */

#else
#error "NOT SUPPORTED TARGET"
#endif
 