#include "../includes/Response.hpp"
#include "../includes/Request.hpp"

// Constructors
Response::Response()
{
	this->_response = "";
	this->_statusCode = 404;
}

Response::Response(int bodyLimit, Request &req, std::vector<Route *> routes)
{
	this->_request = req;
	this->_finalPath = "";
	this->_response = "";
	this->_statusCode = 404;
	this->_request.processRequest();
	this->_routes = routes;
	this->_routeIndex = -1;
	if (bodyLimit > INT_MAX || bodyLimit < 0)
		_bodyLimit = -1;
	else
		this->_bodyLimit = bodyLimit;
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
		case 504:
			body += "504 Gateway Timeout<br>";
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
	std::cout << "error response: " << _response << std::endl;
}

bool	Response::dirListing(std::string &path)
{
	path.erase(path.size() - 1, 1);
	std::cout << "path en dirListing: " << path << std::endl;
	const char *cpath = path.c_str();
	DIR *dir;
	struct dirent *ent;
	std::string body;

	if ((dir = opendir(cpath)) != NULL)
	{
		body = UPPERLISTINGBODY;
		body += "<h2 style=\"text-align:left\"> Directory listing for "
				+ path + "</h2>\n";
		body += "<hr><ul>";
		while ((ent = readdir(dir)) != NULL)
		{
			body += "<li><a href=\"http://localhost:8080"
					+ _request.getPath();
			body += ent->d_name;
			body += "\" style=\"color: #4AF626;\">";
			body += ent->d_name;
			body += "</a></li>\n";
		}
		body += "</ul><hr>";
		closedir(dir);
		body += LOWERLISTINGBODY;
	} 
	else 
		return false;
	this->_response = headerGenerator("200", bodyLen(body));
	this->_response += body + "\r\n\r\n";
	return true;
}

void	Response::applyGetMethod(void)
{
	std::string			body;
	std::stringstream	buffer;
	std::ifstream		file(_finalPath);

	//donde se guarda el cgi que se puede utilizar?
	if (_finalPath.find(this->_routes[this->_routeIndex]->getCgi()) != std::string::npos)
		cgi(_finalPath); 
	else
	{
		buffer << file.rdbuf();
		file.close();
		body = buffer.str();
		body += "\r\n\r\n";
		this->_response = headerGenerator("200", bodyLen(body));
		this->_response += body;
	}

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
		if (this->_routeIndex == -1 || this->_routes[this->_routeIndex]->isDirListing() == false
			|| dirListing(_finalPath) == false)
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

bool	Response::chooseBest(const std::string &rawPath, size_t i, bool &dirList, std::string &root)
{
	// std::cout << "rawPath: " << rawPath << " redir de ruta: " << _routes[i]->getRedir() << std::endl;
	/*
		En caso de que la peticion coincida con la redireccion
		y no sea directory listing, enviar default answer
	*/
	if (dirList == false && this->_routes[i]->getRedir() == rawPath)
	{
		this->_finalPath = _routes[i]->getRoot()
					+ rawPath + _routes[i]->getDefaultAnswer();
		return true;
	}
	else
	{
		// En caso contrario, se solicita directory listing de un directorio literal en ruta
		this->_finalPath = _routes[i]->getRoot() + rawPath;
		return true;
	}
	// Si el metodo del request coincide con el de la ruta
	if (_routes[i]->checkMethod(_request.getMethod()) == true)
	{
		// Si no es directory listing, o es directory listing y la ruta tambien
		if ((dirList == false && _routes[i]->isDirListing())
			|| (dirList == _routes[i]->isDirListing()))
			root = _routes[i]->getRoot();
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
	// std::cout << "rawPath: " << rawPath << " dirList:" << dirList << std::endl;
	for (size_t i = 0; i < vectorSize; i++)
	{
		if (rawPath.find(_routes[i]->getRedir()) == 0)
		{
			if (chooseBest(rawPath, i, dirList, root))
			{
				this->_routeIndex = i;
				return true;
			}
		}
		this->_routeIndex = i;
	}
	if (root == "" || maxCharsFound == 0)
		return false;
	// std::cout << "root: " << root << " rawPath.substr(): " << rawPath.substr(maxCharsFound - 1) << std::endl;
	this->_finalPath = root + rawPath.substr(maxCharsFound - 1);
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
	if ((int)this->_request.getBody().size() > this->_bodyLimit)
	{
		errorResponse(400);
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
	//std::cout << "Sale response!\n";
	// std::cout << "respuesta final: " << this->_response << std::endl;
	return this->_response;
}

void	timeoutHandler(int sig, siginfo_t *info, void *context) {

    std::cout << "Se ha producido un timeout para el PID: " << info->si_pid << std::endl;
    kill(info->si_pid, SIGKILL);
}


std::string Response::cgi(std::string path)
{
	int status;

	//signaction
	struct sigaction	info;
	info.sa_sigaction = timeoutHandler;
	info.sa_flags = SA_SIGINFO;
	sigaction(SIGALRM, &info, NULL);

	//Lo vacio por si hay basura dentro
	this->_response.clear();

	if (access(path.c_str(), F_OK | R_OK) == -1)
	{
		std::cout << "No se ha podido abrir el archivo" << std::endl;
		return (NULL);
	}
	//hard_code Tengo que añadir el ejecutable correspondiente, si es necesario
	//me pasan argv, el path seria el ejecutable y argv seria ejecutable + argumentos
	//En el yaml tengo que añadir cgi_path con el ejecutable

	//Tengo que mirar que tipo de archivo es (HARDCODE PYTHON3)
	char *argv[3];
	if (_finalPath.find(".py") != std::string::npos)
	{
		argv[0] = (char *)"/usr/local/bin/python3";
		argv[1] = (char *)path.c_str();
		argv[2] = 0;
	}
	else if (_finalPath.find(".bin") != std::string::npos)
	{
		argv[0] = (char *)path.c_str();
		argv[1] = (char *)path.c_str();
		argv[2] = 0;
	}
	//ENV
	std::string rMeth;
	std::string sProt;
	std::string pInf;
	rMeth = "REQUEST_METHOD=" + _request.getMethod();
	sProt = "SERVER_PROTOCOL=" + _request.getProtocol();
	pInf = "PATH_INFO=" + _request.getPath();
	char *env[4];
	env[0] = (char *)rMeth.c_str();
	env[1] = (char *)sProt.c_str();
	env[2] = (char *)pInf.c_str();
	env[3] = 0;

	int temp = open(".temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);

	int pid = fork();
	if (!pid)
	{
		if (dup2(temp, STDOUT_FILENO) == -1)
			printf("Error al abrir el pipe");
		close(temp);
		alarm(5);
		if (execve(argv[0], argv, env) == -1)
		{
			std::cout << "La execucion del fork ha fallado." << std::endl;
			exit(1);
		}
		exit(0);
	}
	else
	{
		std::cout << "La creacion del fork ha fallado." << std::endl;
		return (NULL);
	}
	waitpid(pid, &status, 0);
	temp = open(".temp.txt", O_RDONLY);
	char buf[50];
	std::string data;
	ssize_t len = 50;
	while (len > 0)
	{
		len = read(temp, buf, 50);
		data += std::string(buf, len);
	}
	close(temp);
	//Borrar archivo temporal
	remove(".temp.txt");
	data += "\r\n\r\n";
	if (this->_response.empty())
	{
		this->_response = headerGenerator("200", bodyLen(data));
		this->_response += data;
	}
	else
		errorResponse("504");
	return (data);
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
