#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <unistd.h>
# include <map>
# include <sstream>
# include "Response.hpp"

class Request
{
	public:
		// Constructors
		Request();
		Request(std::string);
		Request(const Request &copy);
		
		// Destructor
		~Request();
		
		// Operators
		Request & operator=(const Request &assign);

		// Methods //
		std::string	processRequest(void);
		void		parseFirstLine(void);
		void		parseMethod(int &, int &);
		void		parsePath(int &, int &);
		void		parseProtocol(int &, int &);
		void		parseHeader(void);

		// Setters
		void	setRawRequest(std::string);

		// Getters
		std::string							getRawRequest(void);
		std::string							getMethod(void);
		std::string							getPath(void);
		std::string							getProtocol(void);
		std::string							getBody(void);
		std::string							getHeader(std::string);
		void								printHeaders(void);

	private:
		std::string							_rawRequest;
		std::string							_method;
		std::string							_path;
		std::string							_protocol;
		std::map<std::string,std::string>	_headers;
		std::string							_body;
		Response							_response;
};

#endif

// request header example
/*
GET / HTTP/1.1
Host: localhost:8080
Connection: keep-alive
sec-ch-ua: "Not.A/Brand";v="8", "Chromium";v="114", "Google Chrome";v="114"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "macOS"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br
Accept-Language: es-ES,es;q=0.9,en;q=0.8
*/

