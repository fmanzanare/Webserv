#include "../includes/Response.hpp"
#include "../includes/Request.hpp"

// Constructors
Response::Response()
{
	this->_response = "";
	this->_statusCode = "";
	this->_status = "";
}

Response::Response(Request &req, std::vector<Route *> routes)
{
	this->_request = req;
	this->_finalPath = "";
	this->_response = "";
	this->_statusCode = "";
	this->_status = "";
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
std::string					headerGenerator(const std::string &code, const std::string &bodyLen)
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


// Methods
std::string	Response::bodyResponseCode(const int &code)
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


void		Response::errorResponse(const int &code)
{
	std::stringstream	code_str(code);
	std::stringstream	len_str;
	std::string			body = bodyResponseCode(code);

	code_str << code;
	len_str << body.length();
	this->_response = headerGenerator(code_str.str(), len_str.str());
	this->_response += body;
}

void	Response::applyGetMethod(void)
{
	std::string			body;
	std::stringstream	buffer;
	std::ifstream file(_finalPath);

	if (_finalPath.find(".py") != std::string::npos)
		body = cgi(_finalPath); 
	else
	{
		buffer << file.rdbuf();
		file.close();
		body = buffer.str();
	}
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
		errorResponse(400);
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

void		Response::postResponse(std::string path)
{
	std::vector<std::string> splitted = splitFilePath(path);

	if (splitted.empty() == true)
	{
		errorResponse(204);
		return ;
	}
	// std::ofstream	outputFile(*(splitted.end() - 1));
	// std::cout << *(splitted.end() - 1) << std::endl;
	std::ofstream	outputFile("." + path);
	//std::cout << "." + path << std::endl;
	outputFile << _request.getBody();
	outputFile.close();
	this->_response = headerGenerator("200", "0");

}

void		Response::deleteResponse(std::string path)
{
	//path = "." + path;
	if (std::remove(path.c_str()))
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
		if (rawPath.find(_routes[i]->getRedir()) != std::string::npos)
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
	//std::cout << "Entra response!\n";
	if (this->_request.getProtocol() != "HTTP/1.1")
	{
		errorResponse(426);
		return this->_response;
	}
	if (this->_request.getMethod() == "GET")
		getResponse();
	else if (this->_request.getMethod() == "POST")
		postResponse(this->_request.getPath());
	else if (this->_request.getMethod() == "DELETE")
		deleteResponse(this->_request.getPath());
	else
		errorResponse(405);
	//std::cout << "Sale response!\n";
	return this->_response;
}

std::string Response::cgi(std::string path)
{
	int status;

	if (access(path.c_str(), F_OK | R_OK) == -1)
		return("Onde??");

	//hard_code Tengo que añadir el ejecutable correspondiente, si es necesario
	//me pasan argv, el path seria el ejecutable y argv seria ejecutable + argumentos
	//En el yaml tengo que añadir cgi_path con el ejecutable
	char *argv[3];
	argv[0] = (char *)path.c_str();
	argv[1] = 0;
	argv[2] = 0;
	char *env[4];
	std::string aux;
	aux = "REQUEST_METHOD=" + _request.getMethod();
	env[0] = (char *)aux.c_str();
	//aux = "SERVER_PROTOCOL=" + _request.getProtocol();
	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
	env[2] = (char *)"PATH_INFO=121";
	env[3] = 0;
	//ERROR 500?

	//creo el archivo temporal
	int temp = open(".temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
	//ERROR TIMEOUT 504?
	int pid = fork();

	if (!pid)
	{
		if (dup2(temp, STDOUT_FILENO) == -1)
			printf("Error al abrir el pipe");
		close(temp);
		execve(argv[0], argv, env);
		return ("error");
	}
	waitpid(pid, &status, 0);
	temp = open(".temp.txt", O_RDONLY);
	char buf[50];
	std::string data;
	ssize_t len = -1;
	while (len)
	{
		len = read(temp, buf, 50);
		data += std::string(buf, len);
	}
	close(temp);
	//Borrar archivo temporal
	remove(".temp.txt");
	return (data);
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
