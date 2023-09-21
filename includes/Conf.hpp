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
		int				_port;
		std::string		_host;
		std::string		_serverName;
		//Metodos
        void			setPort(int port);
		void			setHost(std::string host);
		void			setServerName(std::string serverName);
		//void			setError_page();
		//void			setBody_limit();
		//void			Route::Route();
    public:
		//Atributos
        Conf();
        ~Conf();
		//Metodos
		int				getPort(void);
		std::string		getHost(void);
		std::string		getServerName(void);
		//void			getError_page();
		//void			getBody_limit();

        //Conf(Conf const &copy);
        //Conf	&operator=(Conf const &copy);
};

#endif
