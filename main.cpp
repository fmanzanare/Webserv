#include "includes/WebServs.hpp"
#include "includes/Request.hpp"
#include "includes/Response.hpp"
#include <csignal>

// void signalHandler(int signal) {
// 	(void)signal;
// 	std::cout << "Webserv closed!" << std::endl;
// 	std::exit(0);
// }

// void leaks(void) {
// 	system("leaks -q webserv");
// }

// int main(void) {
// 	atexit(leaks);

// 	std::vector<std::string> methods;
// 	methods.push_back("GET");
// 	std::vector<Route *> routes;
// 	routes.push_back(new Route(methods, "./server1", "/", false, "index.html"));
// 	methods.clear();
// 	methods.push_back("POST");
// 	methods.push_back("DELETE");
// 	routes.push_back(new Route(methods, "./server1/storage", "/storage", true, "none"));

// 	std::vector<int> port;
// 	port.push_back(8080);
// 	port.push_back(8081);

// 	WebServs c1;
// 	Server *s1 = new Server(port, routes, "127.0.0.1");
// 	c1.addServer(s1);

// 	routes.clear();
// 	methods.clear();
// 	methods.push_back("GET");
// 	routes.push_back(new Route(methods, "./server2", "/", true, "hello.html"));
// 	methods.clear();
// 	methods.push_back("GET");
// 	routes.push_back(new Route(methods, "./server2/images", "/images", true, "default.html"));

// 	port.clear();
// 	port.push_back(8001);
// 	port.push_back(8002);

// 	Server *s2 = new Server(port, routes, "0.0.0.0");
// 	c1.addServer(s2);

// 	signal(SIGINT, signalHandler);
// 	c1.runWebServs();

// 	return (0);
// }

int main()
{
	Response req = Response();

	req.errorResponse(405);
	std::cout << req.getResponses();
}