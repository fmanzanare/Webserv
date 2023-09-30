#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include "Request.hpp"

# define UPPERDEFBODY "<!DOCTYPE html><html><head></head><body><h1>"
# define LOWERDEFBODY "</h1></body></html>\r\n\r\n"
# define CONTTYPE "Content type: text/html\n"
# define CONTLENGTH "Content length: "

class Response
{
	public:
		// Constructors
		Response();
		Response(Request &req);
		Response(const Response &copy);
		
		// Destructor
		~Response();
		
		// Operators
		Response & operator=(const Response &assign);

		// Methods
		std::string		responseMaker();
		std::string		bodyResponseCode(const int&);
		void			errorResponse(const int&);
		void			getResponse(std::string);
		void			postResponse(std::string);
		void			deleteResponse(std::string);
		void			generateFinalResponse();

		std::string	getResponses(){return _response;} // testing function

	private:
		std::string		_response;
		std::string		_statusCode;
		std::string		_status;
		Request			_request;
};

#endif
// response header example
/*
HTTP/1.1 200 OK\n
Content-Type: text/html\n
Content-Length:  XX\n
\n
[Body]\n\n
*/