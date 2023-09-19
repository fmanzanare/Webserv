#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>

class Response
{
	public:
		// Constructors
		Response();
		Response(const Response &copy);
		
		// Destructor
		~Response();
		
		// Operators
		Response & operator=(const Response &assign);

		// Methods
		std::string	responseMaker(std::string, std::string, std::string);		
		
	private:
		std::string	_response;
		std::string	_statusCode;
		std::string	_status;
		std::string	_contentType;
		std::string _contentLength;
		std::string	_body;
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