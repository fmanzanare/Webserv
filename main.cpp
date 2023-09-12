#include "includes/WebServs.hpp"
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
	Server *s1 = new Server(port, method, "127.0.0.1");

	c1.addServer(s1);

	port.clear();
	port.push_back(8001);
	port.push_back(8002);

	method.clear();
	method.push_back("PUT");

	Server *s2 = new Server(port, method, "0.0.0.0");

	c1.addServer(s2);

	signal(SIGINT, signalHandler);
	c1.runWebServs();

	return (0);
}
