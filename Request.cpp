#include "Request.hpp"

// Constructors
Request::Request()
{
	std::cout << "\e[0;33mDefault Constructor called of Request\e[0m" << std::endl;
}

Request::Request(const Request &copy)
{
	*this = copy;
	std::cout << "\e[0;33mCopy Constructor called of Request\e[0m" << std::endl;
}


// Destructor
Request::~Request()
{
	std::cout << "\e[0;31mDestructor called of Request\e[0m" << std::endl;
}

// Methods
std::string	Request::processRequest(std::string raw)
{
	
}

// Setters
void		Request::setRawRequest(std::string raw)		{_rawRequest = raw;}
void		Request::setMethod(std::string method)		{_method = method;}
void		Request::setPath(std::string path)			{_path = path;}
void		Request::setProtocol(std::string protocol)	{_protocol = protocol;}
void		Request::setHost(std::string host)			{_host = host;}

// Getters
std::string	Request::getRawRequest(void)				{return _rawRequest;}
std::string	Request::getMethod(void)					{return _method;}
std::string	Request::getPath(void)						{return _path;}
std::string	Request::getProtocol(void)					{return _protocol;}
std::string	Request::getHost(void)						{return _host;}
std::string	Request::getBody(void)						{return _body;}

// Operators
Request & Request::operator=(const Request &assign)
{
	this->setRawRequest(assign._rawRequest);
	this->setMethod(assign._method);
	this->setProtocol(assign._protocol);
	return *this;
}

