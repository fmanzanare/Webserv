#include "../includes/Conf.hpp"

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
				Conf::setName(line.substr(line.find("server-name: ") + 13));
			std::getline(archivo, line);
			if (line.find("error-page:") != std::string::npos)
				Conf::setError_page(line.substr(line.find("error-page: ") + 12));
			std::getline(archivo, line);
			if (line.find("body-limit:") != std::string::npos)
				Conf::setError_page(line.substr(line.find("body-limit: ") + 12));
			std::getline(archivo, line);
			if (line.find("host:") != std::string::npos)
				Conf::setHost(line.substr(line.find("host: ") + 6));
			std::getline(archivo, line);
			if (line.find("port:") != std::string::npos)
				Conf::setPorts(line.substr(line.find("port: ") + 6));
			std::getline(archivo, line);
			//Iteramos por las routes
			while (line.find("route:") != std::string::npos)
			{
				std::getline(archivo, line);
				if (line.find("methods:") != std::string::npos)
					Conf::setMethods(line.substr(line.find("methods: ") + 9));
				std::getline(archivo, line);
				if (line.find("directory-listing:") != std::string::npos)
					Conf::setDirListing(line.substr(line.find("directory-listing: ") + 19));
				std::getline(archivo, line);
				if (line.find("default-answer:") != std::string::npos)
					Conf::setDef(line.substr(line.find("default-answer: ") + 16));
				std::getline(archivo, line);
				if (line.find("cgi:") != std::string::npos)
					Conf::setCgi(line.substr(line.find("cgi: ") + 5));
				std::getline(archivo, line);
				if (line.find("redirection:") != std::string::npos)
					Conf::setRedir(line.substr(line.find("redirection: ") + 13));
				std::getline(archivo, line);
				if (line.find("root:") != std::string::npos)
					Conf::setRoot(line.substr(line.find("root: ") + 6));
				//Crear el Router 
				std::getline(archivo, line);
			}
			//Crear el Server
		}
	}
	archivo.close();
}

Conf::~Conf()
{
}
//SETs


int			Conf::setName(std::string name){
	this->_name = name;
	return (1);
}
int			Conf::setError_page(std::string errPage){
	this->_errPage = errPage;
	return (1);
}
int			Conf::setCBodyLimit(int cBodyLimit){
	this->_cBodyLimit = cBodyLimit;
	return (1);
}
int			Conf::setHost(std::string host){
	this->_host = host;
	return (1);
}
int			Conf::setPorts(std::string port){
	//
	return (1);
}

int			Conf::addRoute(Route *route){
	this->_routes.push_back(route);
	return (1);
}

int			Conf::setMethods(std::string method){
	//
	return (1);
}
int			Conf::setDirListing(std::string dirListing){
	if (dirListing.find("true"))
		this->_dirListing = true;
	else if (dirListing.find("false"))
		this->_dirListing = false;
	else
		return (0);
	return (1);
}
int			Conf::setDef(std::string def){
	this->_def = def;
	return (1);
}
int			Conf::setCgi(std::string cgi){
	this->_cgi = cgi;
	return (1);
}
int			Conf::setRedir(std::string redir){
	this->_redir = redir;
	return (1);
}
int			Conf::setRoot(std::string root){
	this->_root = root;
	return (1);
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
