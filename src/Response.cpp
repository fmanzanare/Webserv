#include "../includes/Response.hpp"
#include "../includes/Request.hpp"

// Constructors
Response::Response()
{
	this->_response = "";
	this->_statusCode = 404;
}

Response::Response(Request &req, std::vector<Route *> routes)
{
	this->_request = req;
	this->_finalPath = "";
	this->_response = "";
	this->_statusCode = 404;
	this->_request.processRequest();
	this->_routes = routes;
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
std::vector<std::string>	splitFilePath(std::string path)
{
	std::vector<std::string>	splitted;
	std::stringstream			input(path);
	std::string					subString;

	std::getline(input, subString, '/');
	while (std::getline(input, subString, '/'))
		splitted.push_back(subString);
	return splitted;
}

std::string		bodyLen(std::string body)
{
	std::stringstream body_len;

	body_len << body.length();
	return body_len.str();
}

std::string		headerGenerator(const std::string &code, const std::string &bodyLen)
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

std::string	bodyResponseCode(const int &code)
{
	std::string body = UPPERDEFBODY;

	switch(code)
	{
		case 204:
			body += "204 No Content<br>";
			break;
		case 400:
			body += "400 Bad Request<br>";
			break;
		case 401:
			body += "401 Unauthorized<br>";
			break;
		case 404:
			body += "404 Not Found<br>";
			break;
		case 405:
			body += "405 Method Not Allowed<br>";
			break;
		case 422:
			body += "422 Unprocessable Entity<br>";
			break;
		case 426:
			body += "426 Upgrade Required<br>";
			break;
		default:
			body += "505 Internal Server Error<br>";
			break;
	}
	body += LOWERDEFBODY;
	return body;
}

// Methods
void	Response::errorResponse(const int &code)
{
	std::stringstream	code_str;
	std::stringstream	len_str;
	std::string			body = bodyResponseCode(code);

	code_str << code;
	len_str << body.length();
	this->_response = headerGenerator(code_str.str(), len_str.str());
	this->_response += body;
}

bool	Response::dirListing(std::string &path)
{
	path.erase(path.size() - 1, 1);
	std::cout << "path: " << path << std::endl;
	const char *cpath = path.c_str();
	DIR *dir;
	struct dirent *ent;
	std::string body;

	if ((dir = opendir(cpath)) != NULL)
	{
		// body = UPPERDEFBODY;
		while ((ent = readdir(dir)) != NULL)
			printf("%s\n", ent->d_name);
		closedir(dir);
		// body += LOWERDEFBODY;
	} 
	else 
	{
		std::cout << "Error: directory listing" << std::endl;
		return false;
	}
	return true;
}

void	Response::applyGetMethod(void)
{
	std::string			body;
	std::stringstream	buffer;
	std::ifstream		file(_finalPath);

	buffer << file.rdbuf();
	file.close();
	body = buffer.str();
	body += "\r\n\r\n";
	this->_response = headerGenerator("200", bodyLen(body));
	this->_response += body;
}

/**
 * Implementation of the response for a GET request.
*/
void		Response::getResponse()
{
	if (checkLocation(_request.getPath()) == false)
	{
		errorResponse(_statusCode);
		return ;
	}
	std::cout << "final path: "<<_finalPath<<std::endl;
	if (this->_finalPath.back() == '/')
	{
		if (dirListing(_finalPath) == false)
			errorResponse(404);
		return ;
	}
	if (access(_finalPath.c_str(), F_OK | R_OK) == -1)
	{
		switch(errno)
		{
			case EACCES:
				errorResponse(401);
				break;
			case ENOENT:
				errorResponse(404);
				break;
			default:
				errorResponse(404);
		}
	}
	else
		applyGetMethod();
}

void		Response::postResponse()
{
	// std::vector<std::string> splitted = splitFilePath(path);

	// if (splitted.empty() == true)
	// {
	// 	errorResponse(204);
	// 	return ;
	// }
	if (checkLocation(_request.getPath()) == false)
	{
		errorResponse(400);
		return ;
	}
	std::ofstream	outputFile(_finalPath);
	outputFile << _request.getBody();
	outputFile.close();
	this->_response = headerGenerator("200", "0");

}

void		Response::deleteResponse()
{
	if (checkLocation(_request.getPath()) == false)
	{
		errorResponse(400);
		return ;
	}
	if (std::remove(_finalPath.c_str()))
	{
		errorResponse(422);
		return ;
	}
	this->_response = headerGenerator("200", "0");
}

bool	Response::chooseBest(const std::string &rawPath, size_t &maxCharsFound, size_t i, bool &dirList, std::string &root)
{
	if (rawPath == _routes[i]->getRedir())
	{
		_finalPath = _routes[i]->getRoot()
					+ rawPath + _routes[i]->getDefaultAnswer();
		return true;
	}
	if (maxCharsFound < _routes[i]->getRedir().size()
		&& _routes[i]->checkMethod(_request.getMethod()) == true)
	{
		if ((dirList == false && _routes[i]->isDirListing())
			|| (dirList == _routes[i]->isDirListing()))
		{
			maxCharsFound = _routes[i]->getRedir().size();
			root = _routes[i]->getRoot();
		}
	}
	return false;
}

bool	Response::checkLocation(std::string rawPath)
{
	std::string		redir = "";
	std::string		root = "";
	size_t			maxCharsFound = 0;
	size_t			vectorSize = _routes.size();
	bool			dirList;

	if (rawPath.back() == '/' && rawPath.size() > 1)
		dirList = true;
	else
		dirList = false;

	for (size_t i = 0; i < vectorSize; i++)
	{
		if (rawPath.find(_routes[i]->getRedir()) == 0)
			if (chooseBest(rawPath, maxCharsFound, i, dirList, root)) {return true;}
	}
	if (root == "" || maxCharsFound == 0)
		return false;
	_finalPath = root + rawPath.substr(maxCharsFound - 1);
	return true;
}

/**
 * This function generates a valid http response from the data of the 
 * Request object passed to Response constructor
*/
std::string	Response::responseMaker()
{
	std::cout << "Entra response!\n";
	if (this->_request.getProtocol() != "HTTP/1.1")
	{
		errorResponse(426);
		return this->_response;
	}
	if (this->_request.getMethod() == "GET")
		getResponse();
	else if (this->_request.getMethod() == "POST")
		postResponse();
	else if (this->_request.getMethod() == "DELETE")
		deleteResponse();
	else
		errorResponse(405);
	std::cout << "Sale response!\n";
	return this->_response;
}

// Operators
Response & Response::operator=(const Response &assign)
{
	this->_response = assign._response;
	this->_statusCode = assign._statusCode;
	// this->_status = assign._status;
	this->_request = assign._request;
	return *this;
}
