#include "WebServs.hpp"
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
	std::vector<int> port;
	port.push_back(8080);
	port.push_back(8081);

	std::vector<std::string> method;
	method.push_back("GET");

	WebServs c1;
	c1.addServer(new Server(port, method));

	port.clear();
	port.push_back(1001);
	port.push_back(1002);

	method.clear();
	method.push_back("PUT");
	c1.addServer(new Server(port, method));

	signal(SIGINT, signalHandler);
	c1.runWebServs();

	return (0);
}
