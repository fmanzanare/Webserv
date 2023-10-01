#include "../includes/Request.hpp"

// Constructors
Request::Request()
{
}

Request::Request(std::string raw)
{
	this->_rawRequest = raw;
	this->_method = "";
	this->_path = "";
	this->_protocol = "";
	this->_body = "";
}

Request::Request(const Request &copy)
{
	*this = copy;
}


// Destructor
Request::~Request()
{
}

// Methods
void	Request::parseMethod(int &startPos, int &endPos)
{
	startPos = 0;
	endPos = this->_rawRequest.find(" ");
	if (endPos != (int)std::string::npos)
	{
		this->_method = _rawRequest.substr(startPos, endPos - startPos);
		startPos = endPos + 1;
		return ;
	}
	this->_method = "";
}

void	Request::parsePath(int &startPos, int &endPos)
{
	endPos = this->_rawRequest.find(" ", startPos);
	if (endPos != (int)std::string::npos)
	{
		this->_path = _rawRequest.substr(startPos, endPos - startPos);
		// const char *path = this->_path.c_str();
		startPos = endPos + 1;
		return ;
	}
	this->_path = "";
}

void	Request::parseProtocol(int &startPos, int &endPos)
{
	endPos = this->_rawRequest.find("\r\n", startPos);
	if (endPos != (int)std::string::npos)
	{
		this->_protocol = _rawRequest.substr(startPos, endPos - startPos);
		startPos = endPos + 1;
		return ;
	}
	this->_protocol = "";
}

void	Request::parseFirstLine(void)
{
	int startPos;
	int endPos;

	endPos = 0;
	parseMethod(startPos, endPos);
	parsePath(startPos, endPos);
	parseProtocol(startPos, endPos);
}

void	Request::parseHeader(void)
{
	std::stringstream	input(_rawRequest);
	std::string			line;
	std::string 		firstLine;
	
	std::getline(input, firstLine);
	while (std::getline(input, line))
	{
		std::string key = line.substr(0, line.find(":"));
		std::string value = line.substr(line.find(": ") + 2);
		_headers[key] = value;
		std::cout << line << std::endl;
	}
}

void	Request::processRequest(void)
{
	parseFirstLine();
	parseHeader();
	if (this->_rawRequest.length() - (this->_rawRequest.find("\r\n\r\n") + 4) > 0)
		this->_body = this->_rawRequest.substr(this->_rawRequest.find("\r\n\r\n") + 4);	
}

// Setters
void		Request::setRawRequest(std::string raw) {this->_rawRequest = raw;}

// Getters
std::string							Request::getRawRequest(void)		{return this->_rawRequest;}
std::string							Request::getMethod(void)			{return this->_method;}
std::string							Request::getPath(void)				{return this->_path;}
std::string							Request::getProtocol(void)			{return this->_protocol;}
std::string							Request::getBody(void)				{return _body;}

std::string							Request::getHeader(std::string key)
{
	try {
		return _headers[key];
	} catch(...) {
		return "";
	}
}


// Operators
Request & Request::operator=(const Request &assign)
{
	this->_rawRequest = assign._rawRequest;
	this->_method = assign._method;
	this->_path = assign._path;
	this->_protocol = assign._protocol;
	this->_headers = assign._headers;
	this->_body = assign._body;
	return *this;
}

void Request::printHeaders(void) {
	std::map<std::string, std::string>::iterator it;
	for (it = _headers.begin(); it != _headers.end(); it++) {
		std::cout << "Key: " << it->first << ", value: " << it->second << std::endl;
	}
}
