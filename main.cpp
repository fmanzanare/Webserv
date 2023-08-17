#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


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

	int addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
	if (connection < 0) {
		std::cout << "Error accepting connection" << std::endl;
		exit(1);
	}

	char buffer[1000];
	read(connection, buffer, 1000);
	std::cout << "The message was: " << buffer << std::endl;

	std::string response = "Hello World!";
	send(connection, response.c_str(), response.size(), 0);

	close(connection);
	close(sockfd);

	return (0);
}
