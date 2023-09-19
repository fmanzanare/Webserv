#include "../includes/WebServs.hpp"

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
std::vector<Server *> WebServs::getCluster() { return (this->_cluster); }

void WebServs::getServersSockets() {
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		for (int j = 0; j < (int)this->_cluster[i]->getSockets().size(); j++) {
			this->_wSockets.push_back(this->_cluster[i]->getSockets()[j]);
			std::cout << "Socket " << this->_cluster[i]->getSockets()[j] << " on server " << i << std::endl;
		}
	}
}

// SETTERS:
void WebServs::addServer(Server *s) { this->_cluster.push_back(s); }

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

	this->_clients.clear();
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		std::vector<Client *> clients;
		clients = this->_cluster[i]->getClients();
		for (int j = 0; j < (int)clients.size(); j++) {
			this->_clients.push_back(clients[j]);
		}
	}
	for (int i = 0; i < (int)this->_clients.size(); i++) {
		fds[this->_nfds].fd = this->_clients[i]->getSocket();
		if (this->_clients[i]->isFinishedRequest()) {
			fds[this->_nfds].events = POLLOUT;
			std::cout << "Client socket: " << this->_clients[i]->getSocket() << std::endl;
		}
		else {
			fds[this->_nfds].events = POLLIN;
		}
		this->_nfds++;
	}
}

/**
 * Gets the fd where there is an incomming connection and the server index on the cluster.
 * Accepts the connection and add the new client to the server.
 * @param inFd fd of the pollfd struct where the incomming connection is.
 * @param serverIdx Index of the server on the cluster vector.
*/
void WebServs::acceptPollinConnection(int inFd, int serverIdx) {
	sockaddr_in sockaddr;
	int socklen = sizeof(sockaddr);
	int cSocket = accept(inFd, (struct sockaddr *)&sockaddr, (socklen_t *)&socklen);
	if (cSocket <= 0) {
		throw AcceptConnectionErrorException();
	}
	std::cout << "Accepted incoming client through fd: " << inFd << std::endl;
	std::cout << "Assigned fd for client: " << cSocket << std::endl;
	int flags = fcntl(cSocket, F_SETFL, O_NONBLOCK);
	if (flags < 0) {
		throw FcntlErrorException();
	}
	this->_cluster[serverIdx]->addClient(new Client(cSocket));
}

/**
 * It goes through all the server sockets saved on the pollfd struct.
 * If a new connection is received, the POLLIN revent will be activated.
 * Then a new connection is accepted and saved into the server clients vector.
 * @param *fds pollfd struct where all the sockets fd are save with the events and revents.
*/
void WebServs::checkServersSockets(pollfd *fds) {
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		std::vector<int> serverSockets = this->_cluster[i]->getSockets();
		for (int j = 0; j < (int)serverSockets.size(); j++) {
			for (int k = 0; k < this->_nfds; k++) {
				if (fds[k].revents == POLLIN && fds[k].fd == serverSockets[j]) {
					acceptPollinConnection(fds[k].fd, i);
					break;
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
	for (int i = 0; i < (int)this->_cluster.size(); i++) {
		std::vector<Client *> serverClients = this->_cluster[i]->getClients();
		for (int j = 0; j < (int)serverClients.size(); j++) {
			for (int k = 0; k < this->_nfds; k++) {
				if (fds[k].revents == POLLIN && fds[k].fd == serverClients[j]->getSocket()) {
					serverClients[j]->receiveData();
					/* ----------TESTING---------- */
					if (serverClients[j]->isFinishedRequest())
						std::cout << serverClients[j]->getRequest() << std::endl;
					/* ----------TESTING---------- */
					break;
				}
				if (fds[k].revents == POLLOUT && fds[k].fd == serverClients[j]->getSocket()) {
					// Sending the Response to the client.
					Request req = Request(serverClients[j]->getRequest());
					req.processRequest();
					std::cout << "REQUEST!!!!!!" << std::endl;
					req.printHeaders();
					std::cout << "REQUEST!!!!!!" << std::endl;
					/* ----------TESTING---------- */
					std::string header = "HTTP/1.1 200 OK\n"
										"Content-Type: text/html\n"
										"Content-Length: XX\r\n"
										"\r\n";
					std::string body = "<!DOCTYPE html><html><head></head><body><h1>"
										"Hello World!<br>"
										"</h1></body></html>\r\n\r\n";
					std::string response = header + body;
					/* ----------TESTING---------- */

					serverClients[j]->sendData(response);
					if (serverClients[j]->isFinishedResponse())
						this->_cluster[i]->removeClient(j);
					break;
				}
				if (fds[k].revents == POLLHUP && fds[k].fd == serverClients[j]->getSocket()) {
					this->_cluster[i]->removeClient(j);
					break;
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
		}
	}
}
