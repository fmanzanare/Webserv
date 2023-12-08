#include "includes/WebServs.hpp"
#include "includes/Request.hpp"
#include "includes/Response.hpp"
#include "includes/Conf.hpp"
#include <csignal>

void signalHandler(int signal) {
	(void)signal;
	std::cout << "Webserv closed!" << std::endl;
	std::exit(0);
}

void leaks(void) {
	system("leaks -q webserv");
}

int main(int argc,char **argv) {
	//atexit(leaks);
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

		for (int i = 0; i < (int)servers.size(); i++) {
			c1.addServer(servers[i]);
		}

	signal(SIGINT, signalHandler);
	std::vector<Server *> s = c1.getCluster();
	std::vector<Route *> r = s[1]->getRoutes();
	std::cout << r[1]->getRoot() << std::endl;
	c1.runWebServs();

	return (0);
}
