#include "../includes/Conf.hpp"

class Conf::NoAllowPort : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Port\n");
	}
};

class Conf::NoAllowMethod : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Method\n");
	}
};

class Conf::NoAllowDirListing : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow DirListing\n");
	}
};

class Conf::NoAllowNameServer : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Name Server\n");
	}
};

class Conf::NoAllowHost : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Host\n");
	}
};

class Conf::NoAllowcBodyLimit : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Body Limit\n");
	}
};

class Conf::NoAllowRoot : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Root\n");
	}
};

Conf::Conf(std::string fileName)
{
	std::fstream archivo(fileName);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
    }
	std::string line;
	//Iteramos por los servers
	int i = 0;
	while(std::getline(archivo, line))
	{
		if (line.find("server:") != std::string::npos)
		{
			//Añadimos los parametros del server
			i = 0;
			while (std::getline(archivo, line) && i++ < 5)
			{
				//std::cout << line << std::endl;
				if (line.find("server-name:") != std::string::npos)
					setName(line.substr(line.find_first_not_of(" server-name: ")));
				else if (line.find("error-page:") != std::string::npos)
					setError_page(line.substr(line.find_first_not_of("error-page: ")));
				else if (line.find("body-limit:") != std::string::npos)
					setCBodyLimit(line.substr(line.find_first_not_of("body-limit: ")));
				else if (line.find("host:") != std::string::npos)
					setHost(line.substr(line.find_first_not_of("host: ")));
				else if (line.find("port:") != std::string::npos)
					setPorts(line.substr(line.find_first_not_of("port: ")));
			}
			//Iteramos por las routes
			while (line.find("route:") != std::string::npos)
			{
				i = 0;
				while (std::getline(archivo, line) && i++ < 6)
				{
					//std::cout << line << std::endl;
					if (line.find("methods:") != std::string::npos)
						setMethods(line.substr(line.find_first_not_of("methods: ")));
					else if (line.find("directory-listing:") != std::string::npos)
						setDirListing(line.substr(line.find_first_not_of("directory-listing: ")));
					else if (line.find("default-answer:") != std::string::npos)
						setDef(line.substr(line.find_first_not_of("default-answer: ")));
					else if (line.find("cgi:") != std::string::npos)
						setCgi(line.substr(line.find_first_not_of("cgi: ")));
					else if (line.find("redirection:") != std::string::npos)
						setRedir(line.substr(line.find_first_not_of("redirection: ")));
					else if (line.find("root:") != std::string::npos)
						setRoot(line.substr(line.find_first_not_of("root: ")));
				}
				//Crear el Router
				this->_routes.push_back(new Route(getMethods(), getRedir(), getRoot(), getDirListing(), getDef(), getCgi()));
				this->_cgi.clear();
				this->_methods.clear();
				//std::getline(archivo, line);
			}
			//Crear el Server
			this->_cgi.clear();
			this->_methods.clear();
			this->_servers.push_back(new Server(getName(), getPorts(), getHost(), getError_page(), getCBodyLimit(), this->_routes));
			this->_routes.clear();
			this->_ports.clear();
		}
	}
	archivo.close();
}

Conf::~Conf()
{
}
//Methods
void	Conf::freeServer(void)
{
	for (int i = 0; i < (int)this->_servers.size(); i++)
			delete(this->_servers[i]);
	for (int i = 0; i < (int)this->_routes.size(); i++)
		delete(this->_routes[i]);
}
//SETs
void	Conf::setName(std::string name){
	for (int i = 0; i < (int)this->_servers.size(); i++)
		if (name == this->_servers[i]->getName())
		{
			freeServer();
			throw NoAllowNameServer();
		}
	this->_name = name;
}
void	Conf::setError_page(std::string errPage){
	//comprobar existen
	this->_errPage = errPage;
}
void	Conf::setCBodyLimit(std::string cBodyLimit){
	char *aux = (char *)cBodyLimit.c_str();
	for (int i = 0; i < (int)cBodyLimit.size(); i++)
		{
			if (!std::isdigit(aux[i]))
			{
				freeServer();
				throw NoAllowcBodyLimit();
			}
		}
	int result = std::atoi(aux);
	if (0 > result)
	{
		freeServer();
		throw NoAllowcBodyLimit();
	}
	this->_cBodyLimit = result;
}
void	Conf::setHost(std::string host){
	for (int i = 0; i < (int)this->_servers.size(); i++)
		if (host == this->_servers[i]->getHost())
		{
			freeServer();
			throw NoAllowHost();
		}
	this->_host = host;
}
void	Conf::setPorts(std::string ports){
	std::istringstream iss(ports);
	std::string			token;

	while (std::getline(iss, token, ','))
	{
		for (int i = 0; i < (int)token.size(); i++)
		{
			char *aux = (char *)token.c_str();
			if (!std::isdigit(aux[i]))
			{
				freeServer();
				throw NoAllowPort();
			}
		}
		int result = std::atoi(token.c_str());
		if (2000 >= result)
		{
			freeServer();
			throw NoAllowPort();
		}
		for (int i = 0; i < (int)this->_servers.size(); i++)
		{
			std::vector<int> ports = this->_servers[i]->getPorts();
			for (int j = 0; i < (int)ports[j]; j++)
				if (ports[j] == result)
				{
					freeServer();
					throw NoAllowPort();
				}
		}
		this->_ports.push_back(result);
	}
}

void	Conf::setMethods(std::string method){
	std::istringstream iss(method);
	std::string			token;

	int	nGet = 0;
	int	nPost = 0;
	int	nDelete = 0;
	while (std::getline(iss, token, ','))
	{
		if (token != "GET" && token != "POST" && token != "DELETE")
		{
			freeServer();
			throw NoAllowMethod();
		}
		if (token == "GET" && !nGet++)
			this->_methods.push_back(token);
		else if (token == "POST" && !nPost++)
			this->_methods.push_back(token);
		else if (token == "DELETE" && !nDelete++)
			this->_methods.push_back(token);
		else
		{
			freeServer();
			throw NoAllowMethod();
		}
	}
}

void	Conf::setDirListing(std::string dirListing){
	if (dirListing.find("true"))
		this->_dirListing = true;
	else if (dirListing.find("false"))
		this->_dirListing = false;
	else
	{
		freeServer();
		throw NoAllowDirListing();
	}
}
void	Conf::setDef(std::string def){
	//no repetir
	this->_def = def;
}
void	Conf::setCgi(std::string cgi){
	//hacer
	std::istringstream iss(cgi);
	std::string			token;

	while (std::getline(iss, token, ','))
	{
		this->_cgi.push_back(token);
	}
}
void	Conf::setRedir(std::string redir){
	if (redir.back() != '/')
		redir = redir + '/';
	this->_redir = redir;
}
void	Conf::setRoot(std::string root){
	//No se puede repetir por servidor
	for (int i = 0; i < (int)this->_routes.size(); i++)
		if (this->_routes[i]->getRoot() == root)
		{
			std::cout << "get root: " << this->_routes[i]->getRoot() << " root: " << root << std::endl;
			freeServer();
			throw NoAllowRoot();
		}
	if (root[0] != '.')
		root = '.' + root;
	if (access(root.c_str(), F_OK) == -1)
	{
		std::cout << "acceso denegado" << std::endl;
		std::cout << "root: " << root << std::endl;
		freeServer();
		throw NoAllowRoot();
	}
	this->_root = root;
}

//GET
std::string					Conf::getName(void){
	return (this->_name);
}
std::string					Conf::getError_page(void){
	return (this->_errPage);
}
int							Conf::getCBodyLimit(void){
	return (this->_cBodyLimit);
}
std::string					Conf::getHost(void){
	return (this->_host);
}
std::vector<int>			Conf::getPorts(void){
	return (this->_ports);
}

std::vector<Route *>		Conf::getRoutes(void){
	return (this->_routes);
}

std::vector<std::string>	Conf::getMethods(void){
	return (this->_methods);
}
bool						Conf::getDirListing(void){
	return (this->_dirListing);
}
std::string					Conf::getDef(void){
	return (this->_def);
}
std::vector<std::string>	Conf::getCgi(void){
	return (this->_cgi);
}
std::string					Conf::getRedir(void){
	return (this->_redir);
}
std::string					Conf::getRoot(void){
	return (this->_root);
}

std::vector<Server *>		Conf::getServers(void){
	return (this->_servers);
}


// Conf::Conf(Conf const &copy)
// {
// /* Class::Class(const Class &copy) : someValue(copy.someValue) {} */
// }

// Conf    &Conf::operator=(const Conf &copy)
// {
//     // if (this != &copy) {
//         // someValue = copy.someValue;
//     // }
//     // return *this;
// }
