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
		void		setName(std::string name);
		void		setError_page(std::string errPage);
		void		setCBodyLimit(int cBodyLimit);
		void		setHost(std::string host);
        void		setPorts(std::string ports);

		void		addRoute(Route *route);

		void		setMethods(std::string method);
		void		setDirListing(std::string dirListing);
		void		setDef(std::string def);
		void		setCgi(std::string cgi);
		void		setRedir(std::string redir);
		void		setRoot(std::string root);

    public:
		//C
		class						NoAllowPort;
		class						NoAllowMethod;
		class						NoAllowDirListing;
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
