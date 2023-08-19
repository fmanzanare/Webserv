#include "Server.hpp"

// ORTHODOX CANNONICAL FORM:
Server::Server() {}

Server::Server(std::vector<int> ports, std::vector<std::string> methods) {
	for (int i = 0; i < ports.size(); i++) {
		this->_ports.push_back(ports[i]);
	}
	for (int i = 0; i < methods.size(); i++) {
		this->_methods.push_back(methods[i]);
	}
}

Server::~Server() {}

Server::Server(const Server &cp) {
	this->_name = cp._name;
	for (int i = 0; i < cp._ports.size(); i++) {
		this->_ports.push_back(cp._ports[i]);
	}
	for (int i = 0; i < cp._socks.size(); i++) {
		this->_socks.push_back(cp._socks[i]);
	}
	this->_errPage = cp._errPage;
	this->_cBodyLimit = cp._cBodyLimit;
	for (int i = 0; i < cp._methods.size(); i++) {
		this->_methods.push_back(cp._methods[i]);
	}
	this->_dirListing = cp._dirListing;
	this->_defResponse = cp._defResponse;
	this->_redir = cp._redir;
	this->_servRoute = cp._servRoute;
}

Server &Server::operator=(const Server &cp) {
	this->_name = cp._name;
	this->_ports.clear();
	for (int i = 0; i < cp._ports.size(); i++) {
		this->_ports.push_back(cp._ports[i]);
	}
	this->_socks.clear();
	for (int i = 0; i < cp._socks.size(); i++) {
		this->_socks.push_back(cp._socks[i]);
	}
	this->_errPage = cp._errPage;
	this->_cBodyLimit = cp._cBodyLimit;
	this->_methods.clear();
	for (int i = 0; i < cp._methods.size(); i++) {
		this->_methods.push_back(cp._methods[i]);
	}
	this->_dirListing = cp._dirListing;
	this->_defResponse = cp._defResponse;
	this->_redir = cp._redir;
	this->_servRoute = cp._servRoute;
	return (*this);
}

// SETTERS:
void Server::setName(std::string name) {
	this->_name = name;
}

void Server::addPort(int port) {
	this->_ports.push_back(port);
}

void Server::addSocket(int sock) {
	this->_socks.push_back(sock);
}

void Server::setErrPage(std::string errPage) {
	this->_errPage = errPage;
}

void Server::setCBodyLimit(int cBodyLimit) {
	this->_cBodyLimit = cBodyLimit;
}

void Server::addMethod(std::string method) {
	this->_methods.push_back(method);
}

void Server::setDirListing(bool dirListing) {
	this->_dirListing = dirListing;
}

void Server::setDefResponse(std::string defResponse) {
	this->_defResponse = defResponse;
}

void Server::setRedir(std::string redir) {
	this->_redir = redir;
}

void Server::setServRoute(std::string servRoute) {
	this->_servRoute = servRoute;
}

// GETTERS:
std::string Server::getName(void) {
	return (this->_name);
}

std::vector<int> Server::getPorts(void) {
	return (this->_ports);
}

std::vector<int> Server::getSockets(void) {
	return (this->_socks);
}

std::string Server::getErrPage(void) {
	return (this->_errPage);
}

int Server::getCBodyLimit(void) {
	return (this->_cBodyLimit);
}

std::vector<std::string> Server::getMethods(void) {
	return (this->_methods);
}

bool Server::hasDirListing(void) {
	return (this->_dirListing);
}

std::string Server::getDefResponse(void) {
	return (this->_defResponse);
}

std::string Server::getRedir(void) {
	return (this->_redir);
}

std::string Server::getServRoute(void) {
	return (this->_servRoute);
}
