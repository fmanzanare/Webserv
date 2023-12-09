#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

std::string toString(int num) {
	std::stringstream ss;

	ss << num;
	return (ss.str());
}

int main(void) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Error creating socket" << std::endl;
		exit(1);
	}

	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(1024);

	if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Error binding socket" << std::endl;
		exit(1);
	}

	if (listen(sockfd, 10) < 0) {
		std::cout << "Error listening socket" << std::endl;
		exit(1);
	}

	while(1) {
		int addrlen = sizeof(sockaddr);
		int connection = accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
		if (connection <= 0) {
			std::cout << "Error accepting connection" << std::endl;
			exit(1);
		} else {
			char buffer[1000];
			read(connection, buffer, 1000);
			// std::cout << "The message was: " << buffer << std::endl;

			std::string body =
				"<!DOCTYPE html>"
				"<html>"
				"<head>"
				"</head>"
				"<body><h1>Hello World!</h1><p>This is a testing page!!!!!</p></body>"
				"</html>";

			int bodyLen = body.size();

			std::string header =
				"HTTP/1.1 200 OK\n"
				"Content-Type: text/html\n"
				"Content-Length: " + toString(bodyLen) + "\n"
				"\n";

			std::string reply = header + body;

			int bytesSent;
			bytesSent = send(connection, reply.c_str(), reply.size(), 0);
		}

		close(connection);
	}

	close(sockfd);

	return (0);
}


// poll(fds, 3, 10);
// for (int i = 0; i < 3; i++) {
// 	if (fds[i].revents == POLL_IN)
// }
