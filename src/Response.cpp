#include "../includes/Response.hpp"

// Constructors
Response::Response()
{
	this->_response = "";
	this->_statusCode = "";
	this->_status = "";
	this->_contentType = "";
	this->_contentLength = "";
	this->_body = "";
}

Response::Response(const Response &copy)
{
	*this = copy;
}


// Destructor
Response::~Response()
{
}

// Methods
void	Response::bodyResponseCode(const int &code)
{
	this->_body = "<!DOCTYPE html><html><head></head><body><h1>";
	switch(code)
	{
		case 426:
			this->_body += "426 Upgrade Required<br>";
			break;
		case 405:
			this->_body += "405 Method Not Allowed<br>";
			break;
		default:
			this->_body += "404 Server Error<br>";
	}
}

void	Response::errorResponse(const int &code)
{
	std::stringstream itostr;

	itostr << code;
	this->_response = "HTTP/1.1 " + itostr.str() + " KO\n";
	this->_response += "Content type: text/html\n";
	bodyResponseCode(code);
	this->_body += "</h1></body></html>\r\n\r\n";
	itostr.str("");
	itostr.clear();
	itostr << this->_body.length();
	this->_response += "Content length: " + itostr.str() + "\r\n\r\n";
	this->_response += this->_body;
}

std::string	Response::responseMaker(std::string method, std::string path, std::string protocol)
{
	(void)method;
	(void)path;
	if (protocol != "HTTP/1.1")
		errorResponse(426);
	// if (method == "GET")
	// 	getResponse(path);
	// else if (method == "POST")
	// 	postResponse(path);
	// else if (method == "DELETE")
	// 	deleteResponse(path);
	// else
		errorResponse(405);
	// generateFinalResponse();
	return this->_response;
}

// Operators
Response & Response::operator=(const Response &assign)
{
	this->_response = assign._response;
	this->_statusCode = assign._statusCode;
	this->_status = assign._status;
	this->_contentType = assign._contentType;
	this->_contentLength = assign._contentLength;
	this->_body = assign._body;
	return *this;
}
