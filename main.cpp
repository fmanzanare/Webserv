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

int main(void) {
	atexit(leaks);
	try {
		Conf myConf = Conf();
		std::vector<Server *> servers = myConf.getServers();

		WebServs c1;
		for (int i = 0; i < (int)servers.size(); i++) {
			c1.addServer(servers[i]);
		}

		signal(SIGINT, signalHandler);
		c1.runWebServs();
	} catch(...) {
		std::cout << "Non valid config." << std::endl;
	}

	return (0);
}

// int main()
// {
// 	Response req = Response();

// 	req.errorResponse(426);
// 	std::cout << req.getResponses();
// }
