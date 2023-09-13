#include "includes/WebServs.hpp"
#include "includes/Request.hpp"
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

	std::vector<std::string> methods;
	methods.push_back("GET");
	std::vector<Route *> routes;
	routes.push_back(new Route(methods, "./server1", "/", false, "index.html"));
	methods.clear();
	methods.push_back("POST");
	methods.push_back("DELETE");
	routes.push_back(new Route(methods, "./server1/storage", "/storage", true, "none"));

	std::vector<int> port;
	port.push_back(8080);
	port.push_back(8081);

	WebServs c1;
	Server *s1 = new Server(port, routes, "127.0.0.1");
	c1.addServer(s1);

	routes.clear();
	methods.clear();
	methods.push_back("GET");
	routes.push_back(new Route(methods, "./server2", "/", true, "hello.html"));
	methods.clear();
	methods.push_back("GET");
	routes.push_back(new Route(methods, "./server2/images", "/images", true, "default.html"));

	port.clear();
	port.push_back(8001);
	port.push_back(8002);

	Server *s2 = new Server(port, routes, "0.0.0.0");
	c1.addServer(s2);

// 	signal(SIGINT, signalHandler);
// 	c1.runWebServs();

// 	return (0);
// }
void myMethod() {
	throw "jkasef";
}

int main()
{
	std::string raw = 
"GET / HTTP/1.1\n"
"Host: localhost:8080\n"
"Connection: keep-alive\n"
"sec-ch-ua: \"Not.A/Brand\";v=\"8\", \"Chromium\";v=\"114\", \"Google Chrome\";v=\"114\"\n"
"sec-ch-ua-mobile: ?0\n"
"sec-ch-ua-platform: \"macOS\"\n"
"Upgrade-Insecure-Requests: 1\n"
"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
"Sec-Fetch-Site: none\n"
"Sec-Fetch-Mode: navigate\n"
"Sec-Fetch-User: ?1\n"
"Sec-Fetch-Dest: document\n"
"Accept-Encoding: gzip, deflate, br\n"
"Accept-Language: es-ES,es;q=0.9,en;q=0.8\n\n"
"Hola Mundo\n"
"Como estamo\n";
	Request req = Request(raw);

	req.processRequest();
	std::cout << "method: " << req.getMethod() << " path:" << req.getPath() << " protocol:" << req.getProtocol() << std::endl;
	req.printHeaders();
	std::cout << " body:\n" << req.getBody() << std::endl;
}