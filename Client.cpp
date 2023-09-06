#include "Client.hpp"

// EXCEPTIONS:
class Client::RecvErrorException : std::exception {
	virtual const char *what() const throw() {
		return ("RecvErrorException: Error while receiving data on client");
	}
};

// ORTHODOX CANNONICAL FORM:
Client::Client(void) {}

Client::Client(int socket) {
	this->_socket = socket;
	this->_request = "";
	this->_finishedReq = false;
}

Client::~Client() {
	close(this->_socket);
}

Client::Client(const Client &cp) {
	this->_socket = cp._socket;
	this->_request = cp._request;
	this->_finishedReq = cp._finishedReq;
}

Client &Client::operator=(const Client &cp) {
	this->_socket = cp._socket;
	this->_request = cp._request;
	this->_finishedReq = cp._finishedReq;
	return (*this);
}

// SETTERS:
void Client::setSocket(int socket) { this->_socket = socket; }

void Client::setRequest(std::string request) { this->_request = request; }

void Client::setFinishedRequest(bool finishedReq) { this->_finishedReq = finishedReq; }

// GETTERS:
int Client::getSocket() { return (this->_socket); }

std::string Client::getRequest() { return (this->_request); }

bool Client::isFinishedRequest() { return (this->_finishedReq); }

// METHODS:
void Client::receiveData() {
	char	buffer[BUFFER_SIZE];

	if (recv(this->_socket, buffer, BUFFER_SIZE, 0) == -1) {
		throw RecvErrorException();
	}

	this->_request += buffer;

	if (this->_request.find("\r\n\r\n") == std::string::npos) {
		this->_finishedReq = false;
		return ;
	}

	this->_finishedReq = true;
}
