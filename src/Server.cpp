#include "../includes/Server.hpp"

// EXCEPTIONS:
class Server::SocketCreationErrorException : public std::exception {
	virtual const char *what() const throw() {
		return ("SocketCreationErrorException: Error creating socket\n");
	}
};

class Server::SocketBindErrorException : public std::exception {
	virtual const char *what() const throw() {
		return ("SocketBindErrorException: Error binding socket\n");
	}
};

class Server::SocketListenErrorException : public std::exception {
	virtual const char *what() const throw() {
		return ("SocketListenErrorException: Error listening socket\n");
	}
};

// ORTHODOX CANNONICAL FORM:
Server::Server() {}

Server::Server(std::vector<int> ports, std::vector<std::string> methods) {
	for (int i = 0; i < (int)ports.size(); i++) {
		this->_ports.push_back(ports[i]);
	}
	for (int i = 0; i < (int)methods.size(); i++) {
		this->_methods.push_back(methods[i]);
	}
	openSockets();
}

Server::~Server() {}

Server::Server(const Server &cp) {
	this->_name = cp._name;
	for (int i = 0; i < (int)cp._ports.size(); i++) {
		this->_ports.push_back(cp._ports[i]);
	}
	// this->_host attribute!!
	for (int i = 0; i < (int)cp._socks.size(); i++) {
		this->_socks.push_back(cp._socks[i]);
	}
	this->_errPage = cp._errPage;
	this->_cBodyLimit = cp._cBodyLimit;
	for (int i = 0; i < (int)cp._methods.size(); i++) {
		this->_methods.push_back(cp._methods[i]);
	}
	this->_dirListing = cp._dirListing;
	this->_defResponse = cp._defResponse;
	this->_redir = cp._redir;
	this->_servRoute = cp._servRoute;
	for (int i = 0; i < (int)cp._clients.size(); i++) {
		this->_clients.push_back(cp._clients[i]);
	}
}

Server &Server::operator=(const Server &cp) {
	this->_name = cp._name;
	this->_ports.clear();
	for (int i = 0; i < (int)cp._ports.size(); i++) {
		this->_ports.push_back(cp._ports[i]);
	}
	// this->_host attribute!!
	this->_socks.clear();
	for (int i = 0; i < (int)cp._socks.size(); i++) {
		this->_socks.push_back(cp._socks[i]);
	}
	this->_errPage = cp._errPage;
	this->_cBodyLimit = cp._cBodyLimit;
	this->_methods.clear();
	for (int i = 0; i < (int)cp._methods.size(); i++) {
		this->_methods.push_back(cp._methods[i]);
	}
	this->_dirListing = cp._dirListing;
	this->_defResponse = cp._defResponse;
	this->_redir = cp._redir;
	this->_servRoute = cp._servRoute;
	for (int i = 0; i < (int)cp._clients.size(); i++) {
		this->_clients.push_back(cp._clients[i]);
	}
	return (*this);
}

// SETTERS:
void Server::setName(std::string name) { this->_name = name; }

void Server::addPort(int port) { this->_ports.push_back(port); }

void Server::addSocket(int sock) { this->_socks.push_back(sock); }

void Server::setErrPage(std::string errPage) { this->_errPage = errPage; }

void Server::setCBodyLimit(int cBodyLimit) { this->_cBodyLimit = cBodyLimit; }

void Server::addMethod(std::string method) { this->_methods.push_back(method); }

void Server::setDirListing(bool dirListing) { this->_dirListing = dirListing; }

void Server::setDefResponse(std::string defResponse) { this->_defResponse = defResponse; }

void Server::setRedir(std::string redir) { this->_redir = redir; }

void Server::setServRoute(std::string servRoute) { this->_servRoute = servRoute; }

void Server::addClient(Client *client) { this->_clients.push_back(client); }

void Server::removeClient(int idx) {
	std::vector<Client *>::iterator it = this->_clients.begin() + idx;
	Client *tmp = *it;

	this->_clients.erase(it);
	delete tmp;
}

// GETTERS:
std::string Server::getName(void) { return (this->_name); }

std::vector<int> Server::getPorts(void) { return (this->_ports); }

std::vector<int> Server::getSockets(void) { return (this->_socks); }

std::string Server::getErrPage(void) { return (this->_errPage); }

int Server::getCBodyLimit(void) { return (this->_cBodyLimit); }

std::vector<std::string> Server::getMethods(void) { return (this->_methods); }

bool Server::hasDirListing(void) { return (this->_dirListing); }

std::string Server::getDefResponse(void) { return (this->_defResponse); }

std::string Server::getRedir(void) { return (this->_redir); }

std::string Server::getServRoute(void) { return (this->_servRoute); }

std::vector<Client *> Server::getClients(void) { return (this->_clients); }

// METHODS:
/**
 * It opens the server sockets and binds them to the host addresses.
 * Finally, it prepares the queue to receive connection requests.
*/
void Server::openSockets(void) {
	sockaddr_in	sockaddr;

	for (int i = 0; i < (int)this->_ports.size(); i++) {
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			throw SocketCreationErrorException();
		}
		const int enable = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

		this->_socks.push_back(sockfd);

		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = INADDR_ANY; // MAL! DEBE CONTENER EL HOST DEL SERVIDOR (DIREC. IP).
		sockaddr.sin_port = htons(this->_ports[i]);

		if (bind(this->_socks[i], (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
			throw SocketBindErrorException();
		}

		if (listen(this->_socks[i], 10) < 0) { // Second argument: Maximum length to which the queue of pending connections for sockets may grow
			throw SocketListenErrorException();
		}
	}
}