/**
 *	http - Node.js style http class libarary in C++
 * 
 *	@author : taekwonv@gmail.com
 */


#pragma once
#include <string>
#include <memory>
#include <functional>


namespace http
{
	struct EncodingType
	{
	};

	/**
	 *	This object is created when making a request with http.request(). It is passed to the 'response' event of the request object.
	 */
	template <class StringT>
	class ClientResponse
	{
	public:
		template<class Func>
		void EventHandler_data(Func f);
		template<class Func>
		void EventHandler_end(Func f);
		template<class Func>
		void EventHandler_close(Func f);

		int statusCode() const;	
		std::basic_string<StringT> httpVersion() const;
		std::basic_string<StringT> headers() const;
		void setEncoding(EncodingType);
		void pause();
		void resume();	
	};	

	/**
	 * This object is created internally and returned from http.request(). It represents an in-progress request whose header has already been queued. 
	 */
	template <class StringT>
	class ClientRequest
	{
	public:
		template<class Func>
		void EventHandler_response(Func f) { m_onResponse = f; }
		template<class Func>
		void EventHandler_socket(Func f) { m_onSocket = f; }
		template<class Func>
		void EventHandler_connect(Func f) { m_onConnect = f; }
		template<class Func>
		void EventHandler_upgrade(Func f) { m_onUpgrade = f; }
		template<class Func>
		void EventHandler_continue(Func f) { m_onContinue = f; }

		void write(char *chunk, EncodingType);
		void end(char *chunk, EncodingType);
		void abort();
		template <class Func>
		void setTimeout(int, Func callback);
		/*setNoDelay();
		setSocketKeepAlive();*/

	private:		
		std::function<void (std::unique_ptr<ClientResponse<StringT>>)> m_onResponse;
		std::function<void (std::unique_ptr<ClientResponse<StringT>>)> m_onSocket;
		std::function<void (std::unique_ptr<ClientResponse<StringT>>)> m_onConnect;
		std::function<void (std::unique_ptr<ClientResponse<StringT>>)> m_onUpgrade;
		std::function<void (std::unique_ptr<ClientResponse<StringT>>)> m_onContinue;
	};

	                                                     
	/**
	 * _____ _ _         _   _____                     _   
	 * |     | |_|___ ___| |_| __  |___ ___ _ _ ___ ___| |_ 
	 * |   --| | | -_|   |  _|    -| -_| . | | | -_|_ -|  _|
	 * |_____|_|_|___|_|_|_| |__|__|___|_  |___|___|___|_|  
	 *                                   |_|                
	 */

	typedef enum HTTPMETHOD {GET = 1, POST};

	struct RequestOption
	{
		std::string url;
		HTTPMETHOD	method;
	};
	
	template <class StringT, class Func>
	std::unique_ptr<ClientRequest<StringT>> request(RequestOption, Func callback);
	template <class Func>
	void get(RequestOption, Func callback);	
}

