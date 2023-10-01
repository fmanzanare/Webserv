#ifndef RESPONSE_HPP
# define RESPONSE_HPP

/* ----------- INCLUDES ------------*/
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <cerrno>
# include <cerrno>
# include <vector>
# include "Request.hpp"

/* ------------ DEFINES ------------*/
# define UPPERDEFBODY	"<!DOCTYPE html><html><head></head>\
						<body style=\"background-color: black;\">\
						<h1 style=\"color: #4AF626;\">"

# define LOWERDEFBODY "</h1></body></html>\r\n\r\n"
# define CONTTYPE "Content type: text/html\n"
# define CONTLENGTH "Content length: "

std::vector<std::string>	splitFilePath(std::string path);
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
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length:  XX

[Body]
*/