#include <string>
#include "Request.hpp"

int main()
{
	std::string header = "GET / HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\n";
	Request req = Request(header);
	req.processRequest();
	std::cout << req.getMethod() << std::endl << req.getPath()
				<< std::endl << req.getProtocol() << std::endl;
}