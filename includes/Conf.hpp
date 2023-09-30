#ifndef CONF_HPP
#define CONF_HPP

# include <iostream>
# include <fstream>
# include <string>
# include "../includes/Route.hpp"

class Conf
{
    private:
		//Atributos
		std::string				_name;
		std::vector<int>		_ports;
		std::string				_host;
		std::string				_errPage;
		int						_cBodyLimit;
		std::vector<Route *>	_routes;
		//Metodos
        int			addPort(int port);
		int			addRoute(Route *route)
		int			setHost(std::string host);
		int			setName(std::string serverName);
		int			setError_page(std::string errPage);
		int			setBody_limit(int cBodyLimit);
    public:
		//Atributos
        Conf();
        ~Conf();
		//Metodos
		std::string				getServerName(void);
		std::string				getError_page(void);
		int						getBody_limit(void);
		std::string				getHost(void);
		std::vector<int>		getPorts(void);
		std::vector<Route *>	getRoutes(void);
};

#endif
