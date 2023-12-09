#include "includes/WebServs.hpp"
#include "includes/Request.hpp"
#include "includes/Response.hpp"
#include "includes/Conf.hpp"
#include <csignal>

void signalHandler(int signal) {
	(void)signal;
	// std::cout << "Webserv closed!" << std::endl;
	std::exit(0);
}

void leaks(void) {
	system("leaks -q webserv");
}

int main(int argc,char **argv) {
	// atexit(leaks);
	WebServs c1;
	if (argc != 2)
	{
		std::cout << "Default Configuration" << std::endl;
		//comprobar si existe;
		argv[1] = (char *)"conf.yml";
	}

		std::string arg(argv[1]);
		Conf myConf = Conf(arg);
		std::vector<Server *> servers = myConf.getServers();
		
		std::vector<Server *> serverss = myConf.getServers();
		if (servers.empty())
		{
			std::cout << "Not Servers in Configuration" << std::endl;
			std::cout << "Check your conf." << std::endl;
			return (-1);
		}
		for (int i = 0; i < (int)serverss.size(); i++) {
			c1.addServer(serverss[i]);
		}

	signal(SIGINT, signalHandler);
	c1.runWebServs();

	return (0);
}
