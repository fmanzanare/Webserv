#ifndef CONF_HPP
#define CONF_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include "../includes/Route.hpp"
# include "../includes/Server.hpp"

class Conf
{
    private:
		//Atributos
		std::string					_name;
		std::string					_errPage;
		int							_cBodyLimit;
		std::string					_host;
		std::vector<int>			_ports;

		std::vector<Route *>		_routes;

		std::vector<std::string>	_methods;
		bool 						_dirListing;
		std::string 				_def;
		std::string 				_cgi;
		std::string					_redir;
		std::string					_root;

		std::vector<Server *>		_servers;

		
		//Metodos
		int			setName(std::string name);
		int			setError_page(std::string errPage);
		int			setCBodyLimit(int cBodyLimit);
		int			setHost(std::string host);
        int			setPorts(std::string ports);

		int			addRoute(Route *route);

		int			setMethods(std::string method);
		int			setDirListing(std::string dirListing);
		int			setDef(std::string def);
		int			setCgi(std::string cgi);
		int			setRedir(std::string redir);
		int			setRoot(std::string root);

    public:
		//C
		class						NoAllowPort;
		class						NoAllowMethod;
		//Atributos
        Conf();
        ~Conf();
		//Metodos
		std::string					getName(void);
		std::string					getError_page(void);
		int							getCBodyLimit(void);
		std::string					getHost(void);
		std::vector<int>			getPorts(void);

		std::vector<Route *>		getRoutes(void);

		std::vector<std::string>	getMethods(void);
		bool						getDirListing(void);
		std::string					getDef(void);
		std::string					getCgi(void);
		std::string					getRedir(void);
		std::string					getRoot(void);
		std::vector<Server *>		getServers(void);

};

#endif
