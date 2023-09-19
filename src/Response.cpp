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
std::string	Response::responseMaker(std::string method, std::string path, std::string protocol)
{
	if (method == "GET")
		// llamar funcion para generar respuesta para GET
	else if (method == "POST")
		// llamar funcion para generar respuesta para POST
	else if (methods == "DELETE")
		// llamar funcion para generar respuesta para DELETE
	else
		//pasar codigo de error a funcion que genera la respuesta
	return _response;
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
