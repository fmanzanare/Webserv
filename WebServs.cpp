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

class WebServs::FcntlErrorException : public std::exception {
	virtual const char *what() const throw() {
		return ("FcntlErrorException: Error while setting a non-blocking socket\n");
	}
};

// ORTHODOX CANNONICAL FORM:
WebServs::WebServs() {}

WebServs::WebServs(std::vector<Server *> cluster) {
	for (int i = 0; i < (int)cluster.size(); i++) {
		this->_cluster.push_back(cluster[i]);
	}
	this->_nfds = 0;
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
			std::cout << "Socket " << this->_cluster[i]->getSockets()[j] << " on server " << i << std::endl;
		}
	}
}

// SETTERS:
void WebServs::addServer(Server *s) {
	this->_cluster.push_back(s);
}

// METHODS:
/**
 * Get all the openned sockets (server and clients) from each server and save them on the pollfd struct.
 * Firstly, get all the sever sockets and save them.
 * Secondly, clears the cSockets std::vector<int>.
 * Then gets the server clients sockets and adds the to the cScokets std::vector<int>.
 * Finally, adds all the cScokets to the pollfd struct.
 * @param *fds pollfd struct where all the sockets fd are save with the events and revents.
*/
void WebServs::addSocketsToPoll(pollfd *fds) {
	this->_nfds = 0;
	for (int i = 0; i < (int)this->_wSockets.size(); i++) {
		fds[i].fd = this->_wSockets[i];
		fds[i].events = POLLIN;
		this->_nfds++;
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
		fds[this->_nfds].fd = this->_cSockets[i];
		// if (request_incoming is finished == true)
		fds[this->_nfds].events = POLLOUT;
		// else
		// fds[this->_nfds].events = POLLIN;
		this->_nfds++;
	}
}

/**
 * It goes through all the server sockets saved on the pollfd struct.
 * If a new connection is received, the POLLIN revent will be activated.
 * Then a new connection is accepted and saved into the server clients sockets std::vector<int>.
 * @param *fds pollfd struct where all the sockets fd are save with the events and revents.
*/
void WebServs::checkServersSockets(pollfd *fds) {
	for (int i = 0; i < this->_nfds; i++) {
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
						std::cout << "Accepted incoming client through fd: " << fds[i].fd << std::endl;
						std::cout << "Assigned fd for client: " << cSocket << std::endl;
						int flags = fcntl(cSocket, F_SETFL, O_NONBLOCK);
						if (flags < 0) {
							throw FcntlErrorException();
						}
						this->_cluster[j]->addCSocket(cSocket);
						break ;
					}
				}
			}
		}
	}
}

/**
 * It checks all the clients sockets to see if any action can be taken on them.
 * POLLOUT is activated when the socket is ready to send data through.
 * POLLHUP is activated when the client closes his connection from his side.
 * POLLIN is activated when there is outstanding data to be read from the client socket.
 * @param *fds pollfd struct where all the sockets fd are save with the events and revents.
*/
void WebServs::checkClientsSockets(pollfd *fds) {
	for (int i = 0; i < this->_nfds; i++) {
		if (fds[i].revents == POLLOUT) {
			for (int j = 0; j < (int)this->_cluster.size(); j++) {
				std::vector<int> tmp = this->_cluster[j]->getCSockets();
				for (int k = 0; k < (int)tmp.size(); k++) {
					if (tmp[k] == fds[i].fd) {
						/* ------------ TESTING CODE ------------ */
						// char buffer[1000];
						// read(tmp[k], buffer, 1000);
						// std::string str(buffer);
						// std::cout << "The message was: " << str.substr(0, 20) << std::endl;
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
						bytesSent = send(tmp[k], reply.c_str(), reply.size(), 0);
						close(this->_cluster[j]->removeCSocket(tmp[k]));
						/* ------------ TESTING CODE ------------ */
						break ;
					}
				}
			}
		}
		// if (fds[i].revents == POLLIN) {
		// 	for (int j = 0; j < (int)this->_cluster.size(); j++) {
		// 		std::vector<int> tmp = this->_cluster[j]->getCSockets();
		// 		for (int k = 0; k < (int)tmp.size(); k++) {
		// 			if (tmp[k] == fds[i].fd) {

		// 				¡¡¡READING AND PARSING REQUEST SEQUENTIALLY!!!

		// 				break ;
		// 			}
		// 		}
		// 	}
		// }
		if (fds[i].revents == POLLHUP) {
			for (int j = 0; j < (int)this->_cluster.size(); j++) {
				std::vector<int> tmp = this->_cluster[j]->getCSockets();
				for (int k = 0; k < (int)tmp.size(); k++) {
					if (tmp[k] == fds[i].fd) {
						/* ------------ TESTING CODE ------------ */
						close(this->_cluster[j]->removeCSocket(tmp[k]));
						/* ------------ TESTING CODE ------------ */
						break ;
					}
				}
			}
		}
	}
}

/**
 * This function starts the Web server logic.
 * Its flow is as follows:
 * Gets all the servers sockets.
 * Add them to the pollfd struct.
 * Check the poll() function to see if there is any socket with outstanding actions.
 * Checks servers sockets to add new incoming connections.
 * Checks clients sockets to read or write or them if they are ready to.
*/
void WebServs::runWebServs() {
	getServersSockets();

	pollfd fds[MAX_CONNECTIONS];

	while (true) {
		std::memset(fds, 0, sizeof(fds));
		addSocketsToPoll(fds);
		int ret = poll(fds, this->_nfds, (2 * 1000));
		if (ret == -1) {
			throw PollErrorException();
		}
		if (ret) {
			checkServersSockets(fds);
			checkClientsSockets(fds);


			// /* ------------ TESTING CODE ------------ */
			// std::cout << "Server sockets: ";
			// for (int i = 0; i < (int)this->_wSockets.size(); i++) {
			// 	std::cout << this->_wSockets[i] << " ";
			// }
			// std::cout << std::endl;
			// std::cout << "Client sockets: ";
			// for (int i = 0; i < (int)this->_cSockets.size(); i++) {
			// 	std::cout << this->_cSockets[i] << " ";
			// }
			// std::cout << std::endl;
			// /* ------------ TESTING CODE ------------ */
		}
	}
}
