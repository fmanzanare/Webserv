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

class WebServs::AcceptConnectionErrorException : public std::exception {
	virtual const char *what() const throw() {
		return ("AcceptConnectionErrorException: Error while accepting client connection\n");
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
			this->_wSockets.push_back(this->_cluster[i]->getSockets()[j]);
		}
	}
}

// SETTERS:
void WebServs::addServer(Server *s) {
	this->_cluster.push_back(s);
}

// METHODS:
int WebServs::addSocketsToPoll(pollfd *fds) {
	int nfds = 0;
	for (int i = 0; i < (int)this->_wSockets.size(); i++) {
		fds[i].fd = this->_wSockets[i];
		fds[i].events = POLLIN;
		nfds++;
	}

	this->_cSockets.clear();
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		std::vector<int> clients;
		clients = this->_cluster[i]->getCSockets();
		for (int j = 0; j < (int)clients.size(); j++) {
			this->_cSockets.push_back(clients[j]);
		}
	}
	for (int i = 0; i < (int)this->_cSockets.size(); i++) {
		fds[nfds].fd = this->_cSockets[i];
		fds[nfds].events = POLLIN | POLLOUT | POLLHUP;
		nfds++;
	}

	return (nfds);
}

void WebServs::checkServersSockets(pollfd *fds, int nfds) {
	for (int i = 0; i < nfds; i++) {
		if (fds[i].revents == POLLIN) {
			for (int j = 0; j < (int)this->_cluster.size(); j++) {
				std::vector<int> tmp = this->_cluster[j]->getSockets();
				for (int k = 0; k < (int)tmp.size(); k++) {
					if (tmp[k] == fds[i].fd) {
						sockaddr_in sockaddr;
						int socklen = sizeof(sockaddr);
						int cSocket = accept(fds[i].fd, (struct sockaddr *)&sockaddr, (socklen_t *)&socklen);
						if (cSocket <= 0) {
							throw AcceptConnectionErrorException();
						}
						this->_cluster[j]->addCSocket(cSocket);



						/* ------------ TESTING CODE ------------ */
						char buffer[1000];
						read(cSocket, buffer, 1000);
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
						bytesSent = send(cSocket, reply.c_str(), reply.size(), 0);
						close(this->_cluster[j]->removeCSocket(cSocket));
						/* ------------ TESTING CODE ------------ */
						break ;
					}
				}
			}
		}
	}
}

void WebServs::runWebServs() {
	getServersSockets();

	pollfd fds[MAX_CONNECTIONS];

	while (true) {
		std::memset(fds, 0, sizeof(fds));
		int nfds = addSocketsToPoll(fds);
		int ret = poll(fds, nfds, (10 * 1000));
		if (ret == -1) {
			throw PollErrorException();
		} else if (ret) {
			checkServersSockets(fds, nfds);
			// checkClientsSockets(fds, nfds);


			/* ------------ TESTING CODE ------------ */
			std::cout << "Server sockets: ";
			for (int i = 0; i < (int)this->_wSockets.size(); i++) {
				std::cout << this->_wSockets[i] << " ";
			}
			std::cout << std::endl;
			std::cout << "Client sockets: ";
			for (int i = 0; i < (int)this->_cSockets.size(); i++) {
				std::cout << this->_cSockets[i] << " ";
			}
			std::cout << std::endl;
			/* ------------ TESTING CODE ------------ */
		}
	}
}
