#ifndef RESPONSE_HPP
# define RESPONSE_HPP

/* ----------- INCLUDES ------------*/
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <cerrno>
# include <cstdio>
# include <cstdlib>
# include <vector>
# include <filesystem>
# include <dirent.h>
# include "Request.hpp"
# include "Route.hpp"

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
		Response(Request &req, std::vector<Route *>);
		Response(const Response &copy);
		
		// Destructor
		~Response();

		// Methods
		std::string		responseMaker();
		
		// Operators
		Response & operator=(const Response &assign);

		std::string	getResponses(){return _response;} // testing function

	private:
		// Attributes
		int						_statusCode;
		std::string				_response;
		std::string				_finalPath;
		std::vector<Route *>	_routes;
		Request					_request;

		// Methods
		void			errorResponse(const int&);
		void			getResponse(void);
		void			postResponse(void);
		void			deleteResponse(void);
		void			generateFinalResponse(void);
		bool			chooseBest(const std::string &, size_t &, size_t i, bool &, std::string &);
		bool			checkLocation(std::string);
		void			applyGetMethod(void);
		bool			dirListing(std::string&);
};

#endif
// response header example
/*
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length:  XX

[Body]
*/
