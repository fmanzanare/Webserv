#include "../includes/Conf.hpp"

Conf::Conf()
{
	std::fstream archivo("conf.yml");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        
    }
	std::string line;

	while(std::getline(archivo, line))
	{
		if (line.find("host:") != std::string::npos)
		{
			size_t position = line.find('\r');
			if (position != std::string::npos)
				line.erase(position);
			Conf::setHost(line.substr(line.find("host: ") + 6));
		}
		else if (line.find("port:") != std::string::npos)
		{
			size_t position = line.find('\r');
			if (position != std::string::npos)
				line.erase(position);
			Conf::setPort(std::stoi(line.substr(line.find("port: ") + 6)));
		}
		else if (line.find("server-name:") != std::string::npos)
		{
			size_t position = line.find('\r');
			if (position != std::string::npos)
				line.erase(position);
			Conf::setServerName(line.substr(line.find("server-name: ") + 13));
		}
	}
	archivo.close();
}

Conf::~Conf()
{
}
//SETs


void    Conf::setPort(int port)
{
	this->_port = port;
}

void	 Conf::setHost(std::string host)
{
	this->_host = host;
}

void	 Conf::setServerName(std::string serverName)
{
	this->_serverName = serverName;
}

//GET
int				getBody_limit(void)
{
	return (this->_cBodyLimit);
}
std::vector<Route *>	getRoutes(void)
{
	return (this->_routes);
}
std::vector<int>		getPorts(void)
{
	return (this->_ports);
}
std::string	 Conf::getHost(void)
{
	return (this->_host);
}
std::string	 Conf::getServerName(void)
{
	return (this->_serverName);
}
std::string				getError_page(void)
{
	return (this->_errPage);
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
