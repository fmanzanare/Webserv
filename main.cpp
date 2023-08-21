#include "WebServs.hpp"

int main(void) {
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

	c1.runWebServs();

	return (0);
}
