#include "../includes/Response.hpp"
#include "../includes/Request.hpp"

// Constructors
Response::Response()
{
	this->_response = "";
	this->_statusCode = "";
	this->_status = "";
}

Response::Response(Request &req)
{
	this->_request = req;
	this->_response = "";
	this->_statusCode = "";
	this->_status = "";
	req.processRequest();
	this->_request = req;
}

Response::Response(const Response &copy)
{
	*this = copy;
}


// Destructor
Response::~Response()
{
}

// Non member functions
std::string	headerGenerator(const std::string &code, const std::string &bodyLen)
{
	std::string header;

	if (code == "200")
		header = "HTTP/1.1 " + code + " OK\n";
	else
		header = "HTTP/1.1 " + code + " KO\n";
	header += CONTTYPE;
	header += CONTLENGTH + bodyLen + "\r\n\r\n";
	return header;
}

// Methods
std::string	Response::bodyResponseCode(const int &code)
{
	std::string body = UPPERDEFBODY;

	switch(code)
	{
		case 426:
			body += "426 Upgrade Required<br>";
			break;
		case 405:
			body += "405 Method Not Allowed<br>";
			break;
		default:
			body += "404 Server Error<br>";
	}
	body += LOWERDEFBODY;
	return body;
}


void	Response::errorResponse(const int &code)
{
	std::stringstream	code_str(code);
	std::stringstream	len_str;
	std::string			body = bodyResponseCode(code);

	code_str << code;
	len_str << body.length();
	this->_response = headerGenerator(code_str.str(), len_str.str());
	this->_response += body;
}

/**
 * Implementation of the response for a GET request.
*/
void	Response::getResponse(std::string path)
{
	std::string			body;
	std::stringstream	buffer;
	std::stringstream	body_len;

	path.insert(0, 1, '.');
	if (access(path.c_str(), F_OK | W_OK) == 0)
	{
		std::ifstream file(path);
		buffer << file.rdbuf();
		body = buffer.str();
		body += "\r\n\r\n";
		body_len << body.length();
		this->_response = headerGenerator("200", body_len.str());
		this->_response += body;
	}
}

/**
 * This function generates a valid http response from the data of the 
 * Request object passed to Response constructor
*/
std::string	Response::responseMaker()
{
	if (this->_request.getProtocol() != "HTTP/1.1")
	{
		errorResponse(426);
		return this->_response;
	}
	
	if (this->_request.getMethod() == "GET")
		getResponse(this->_request.getPath());
	// else if (method == "POST")
	// 	postResponse(path);
	// else if (method == "DELETE")
	// 	deleteResponse(path);
	else
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
	this->_request = assign._request;
	return *this;
}
