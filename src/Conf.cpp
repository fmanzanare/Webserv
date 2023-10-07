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
Conf::Conf()
{
	std::fstream archivo("conf.yml");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;

    }
	std::string line;
	//Iteramos por los servers
	while(std::getline(archivo, line))
	{
		if (line.find("server:") != std::string::npos)
		{
			//Añadimos los parametros del server
			std::getline(archivo, line);
			if (line.find("server-name:") != std::string::npos)
				setName(line.substr(line.find("server-name: ") + 13));
			std::getline(archivo, line);
			if (line.find("error-page:") != std::string::npos)
				setError_page(line.substr(line.find("error-page: ") + 12));
			std::getline(archivo, line);
			if (line.find("body-limit:") != std::string::npos)
				setCBodyLimit(std::stoi(line.substr(line.find("body-limit: ") + 12)));
			std::getline(archivo, line);
			if (line.find("host:") != std::string::npos)
				setHost(line.substr(line.find("host: ") + 6));
			std::getline(archivo, line);
			if (line.find("port:") != std::string::npos)
				setPorts(line.substr(line.find("port: ") + 6));
			std::getline(archivo, line);
			//Iteramos por las routes
			while (line.find("route:") != std::string::npos)
			{
				std::getline(archivo, line);
				if (line.find("methods:") != std::string::npos)
					setMethods(line.substr(line.find("methods: ") + 9));
				std::getline(archivo, line);
				if (line.find("directory-listing:") != std::string::npos)
					setDirListing(line.substr(line.find("directory-listing: ") + 19));
				std::getline(archivo, line);
				if (line.find("default-answer:") != std::string::npos)
					setDef(line.substr(line.find("default-answer: ") + 16));
				std::getline(archivo, line);
				if (line.find("cgi:") != std::string::npos)
					setCgi(line.substr(line.find("cgi: ") + 5));
				std::getline(archivo, line);
				if (line.find("redirection:") != std::string::npos)
					setRedir(line.substr(line.find("redirection: ") + 13));
				std::getline(archivo, line);
				if (line.find("root:") != std::string::npos)
					setRoot(line.substr(line.find("root: ") + 6));
				//Crear el Router
				this->_routes.push_back(new Route(getMethods(), getRedir(), getRoot(), getDirListing(), getDef()));
				std::getline(archivo, line);
			}
			//Crear el Server
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
//SETs


void	Conf::setName(std::string name){
	this->_name = name;
}
void	Conf::setError_page(std::string errPage){
	this->_errPage = errPage;
}
void	Conf::setCBodyLimit(int cBodyLimit){
	this->_cBodyLimit = cBodyLimit;
}
void	Conf::setHost(std::string host){
	this->_host = host;
}
void	Conf::setPorts(std::string ports){
	std::istringstream iss(ports);
	std::string			token;

	while (std::getline(iss, token, ','))
	{
		int result = std::atoi(token.c_str());
		if (2000 >= result)
			throw NoAllowPort();
		this->_ports.push_back(result);
	}
}

void	Conf::addRoute(Route *route){
	this->_routes.push_back(route);
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
			throw NoAllowMethod();
		if (token == "GET" && !nGet++)
			this->_methods.push_back(token);
		else if (token == "POST" && !nPost++)
			this->_methods.push_back(token);
		else if (token == "DELETE" && !nDelete++)
			this->_methods.push_back(token);
		else	throw NoAllowMethod();
	}
}
void	Conf::setDirListing(std::string dirListing){
	if (dirListing.find("true"))
		this->_dirListing = true;
	else if (dirListing.find("false"))
		this->_dirListing = false;
	else	throw NoAllowDirListing();
}
void	Conf::setDef(std::string def){
	this->_def = def;
}
void	Conf::setCgi(std::string cgi){
	this->_cgi = cgi;
}
void	Conf::setRedir(std::string redir){
	this->_redir = redir;
}
void	Conf::setRoot(std::string root){
	this->_root = root;
}

//GET
std::string				Conf::getName(void){
	return (this->_name);
}
std::string				Conf::getError_page(void){
	return (this->_errPage);
}
int						Conf::getCBodyLimit(void){
	return (this->_cBodyLimit);
}
std::string				Conf::getHost(void){
	return (this->_host);
}
std::vector<int>		Conf::getPorts(void){
	return (this->_ports);
}

std::vector<Route *>	Conf::getRoutes(void){
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
std::string					Conf::getCgi(void){
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