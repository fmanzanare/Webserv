#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>

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
		void		bodyResponseCode(const int&);
		void		errorResponse(const int&);
		void		getResponse(std::string);
		void		postResponse(std::string);
		void		deleteResponse(std::string);
		void		generateFinalResponse();
		
		std::string	getResponses(){return _response;}

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