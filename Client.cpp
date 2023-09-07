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
	char	buffer[BUFFER_SIZE + 1] = {0};

	if (recv(this->_socket, buffer, BUFFER_SIZE, 0) == -1) {
		throw RecvErrorException();
	}

	this->_request += buffer;

	size_t pos = this->_request.find("\r\n\r\n");
	if (pos == std::string::npos) {
		this->_finishedReq = false;
		return ;
	}

	size_t contLenPos = this->_request.find("Content-Length: ");
	if (contLenPos == std::string::npos) {
		if (this->_request.find("Transfer-Encoding: chunked") != std::string::npos) {
			if (this->_request.substr(this->_request.size() - 5) == "0\r\n\r\n") {
				this->_finishedReq = true;
				return ;
			} else {
				this->_finishedReq = false;
				return ;
			}
		}
		this->_finishedReq = true;
		return ;
	}

	size_t newLine = this->_request.find("\n", contLenPos);
	std::string tmp = this->_request.substr((contLenPos + 16), ((contLenPos + 16) - (newLine)));
	int bodyLen = std::atoi(tmp.c_str());

	if (this->_request.size() >= bodyLen + pos + 4) {
		this->_finishedReq = true;
		return ;
	}

	this->_finishedReq = false;
}
