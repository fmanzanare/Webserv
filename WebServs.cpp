#include "WebServs.hpp"

std::string toString(int num) {
	std::stringstream ss;

	ss << num;
	return (ss.str());
}

// EXCEPTIONS:
class WebServs::PollErrorException : public std::exception {
	virtual const char *what() const throw() {
		return ("PollErrorException: Error while waiting sockets.\n");
	}
};

// ORTHODOX CANNONICAL FORM:
WebServs::WebServs() {}

WebServs::WebServs(std::vector<Server *> cluster) {
	for (int i = 0; i < (int)cluster.size(); i++) {
		this->_cluster.push_back(cluster[i]);
	}
}

WebServs::~WebServs() {
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		delete this->_cluster[i];
	}
	this->_cluster.clear();
}

WebServs::WebServs(const WebServs &cp) {
	for (int i = 0; i < (int)cp._cluster.size(); i++) {
		this->_cluster.push_back(cp._cluster[i]);
	}
}

WebServs &WebServs::operator=(const WebServs &cp) {
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		delete this->_cluster[i];
	}
	this->_cluster.clear();
	for (int i = 0; i < (int)cp._cluster.size(); i++) {
		this->_cluster.push_back(cp._cluster[i]);
	}
	return (*this);
}

// GETTERS:
std::vector<Server *> WebServs::getCluster() {
	return (this->_cluster);
}

void WebServs::getServersSockets() {
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		for (int j = 0; j < (int)this->_cluster[i]->getSockets().size(); j++) {
			this->_sockets.push_back(this->_cluster[i]->getSockets()[j]);
		}
	}

	for (int i = 0; i < (int)this->_sockets.size(); i++) {
		std::cout << this->_sockets[i] << " ";
	}
	std::cout << std::endl;
}

// SETTERS:
void WebServs::addServer(Server *s) {
	this->_cluster.push_back(s);
}

// METHODS:
int WebServs::addServersSocketsToPoll(pollfd *fds) {
	int i;
	for (i = 0; i < (int)this->_sockets.size(); i++) {
		fds[i].fd = this->_sockets[i];
		fds[i].events = POLLIN | POLLOUT | POLLHUP;
	}
	return (i);
}

void WebServs::checkServersSockets(pollfd *fds, int nfds) {
	for (int i = 0; i < nfds; i++) {
		if (fds[i].revents == POLLIN) {
			sockaddr_in sockaddr;
			int socklen = sizeof(sockaddr);
			// GUARDAR SOCKET CLIENTE EN CONTENEDOR DENTRO DEL SERVER.
			/* ------------ TESTING CODE ------------ */
			int connection = accept(fds[i].fd, (struct sockaddr *)&sockaddr, (socklen_t *)&socklen);
			if (connection <= 0) {
				std::cout << "Error accepting connection" << std::endl;
				exit(1);
			} else {
				char buffer[1000];
				read(connection, buffer, 1000);
				std::string str(buffer);
				std::cout << "The message was: " << str.substr(0, 20) << std::endl;

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
			/* ------------ TESTING CODE ------------ */
		}
	}
}

void WebServs::runWebServs() {
	getServersSockets();

	pollfd fds[MAX_CONNECTIONS];

	while (true) {
		std::memset(fds, 0, sizeof(fds));
		int nfds = addServersSocketsToPoll(fds);
		int ret = poll(fds, nfds, (10 * 1000));
		std::cout << "Timeout\n";
		if (ret == -1) {
			std::cout << "Error on poll()." << std::endl;
			throw PollErrorException();
		} else if (ret) {
			checkServersSockets(fds, nfds);
			// checkClientsSockets(fds, nfds);
		}
	}
}
