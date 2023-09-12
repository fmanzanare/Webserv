#include "../includes/Request.hpp"

// Constructors
Request::Request()
{
}

Request::Request(std::string raw)
{
	this->_rawRequest = raw;
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
bool	Request::parseMethod(int &startPos, int &endPos)
{
	startPos = 0;
	endPos = this->_rawRequest.find(" ");
	if (endPos != (int)std::string::npos)
	{
		this->_method = _rawRequest.substr(startPos, endPos - startPos);
		if (this->_method != "GET" && this->_method != "PUT" && this->_method != "DELETE")
			return false;
		startPos = endPos + 1;
	}
	return true;
}

bool	Request::parsePath(int &startPos, int &endPos)
{
	endPos = this->_rawRequest.find(" ", startPos);
	if (endPos != (int)std::string::npos)
	{
		this->_path = _rawRequest.substr(startPos, endPos - startPos);
		const char *path = this->_path.c_str();
		if (access(path, F_OK | R_OK) == -1)
			return false;
		startPos = endPos + 1;
	}
	return true;
}

bool	Request::parseProtocol(int &startPos, int &endPos)
{
	endPos = this->_rawRequest.find("\n", startPos);
	if (endPos != (int)std::string::npos)
	{
		this->_protocol = _rawRequest.substr(startPos, endPos - startPos);
		if (this->_protocol != "HTTP/1.1")
			return false;
		startPos = endPos + 1;
	}
	return true;
}

bool	Request::parseFirstLine(void)
{
	int startPos;
	int endPos;

	endPos = 0;
	if (parseMethod(startPos, endPos) == false)
		return false;
	if (parsePath(startPos, endPos) == false)
		return false;
	if (parseProtocol(startPos, endPos) == false)
		return false;
	return true;
}

bool	Request::parseHeader(void)
{
	std::stringstream	input(_rawRequest);
	std::string			line;
	std::string firstLine;
	std::getline(input, firstLine);
	std::getline(input, line);
	while (!line.empty())
	{
		std::string key = line.substr(0, line.find(":"));
		std::string value = line.substr(line.find(": ") + 2);
		_headers[key] = value;
		std::getline(input, line);
	}
	return true;
}

std::string	Request::processRequest(void)
{
	if (_rawRequest.empty() == false && parseFirstLine() == false)
	{
		std::cout << "Non valid header\n";
		return "";
	}
	if(parseHeader() == false)
	{
		std::cout << "Non valid header\n"; 	
		return "";
	}
	return _method;
}

// Setters
void		Request::setRawRequest(std::string raw) {this->_rawRequest = raw;}

// Getters
std::string							Request::getRawRequest(void)	{return this->_rawRequest;}
std::string							Request::getMethod(void)		{return this->_method;}
std::string							Request::getPath(void)			{return this->_path;}
std::string							Request::getProtocol(void)		{return this->_protocol;}
std::string							Request::getBody(void)			{return _body;}
std::string	Request::getHeader(std::string key)		{
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
	this->_body = assign._body;
	return *this;
}

void Request::printHeaders(void) {
	std::map<std::string, std::string>::iterator it;
	for (it = _headers.begin(); it != _headers.end(); it++) {
		std::cout << "Key: " << it->first << ", value: " << it->second << std::endl;
	}
}