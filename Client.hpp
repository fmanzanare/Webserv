#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <cstdlib>

#define BUFFER_SIZE 1000

class Client {
	private:
		// ATTRIBUTES:
		int				_socket;
		std::string		_request;
		bool			_finishedReq;

		// PRIVATE CONSTRUCTOR:
		Client();

	public:
		// ORTHODOX CANNONICAL FORM:
		Client(int socket);
		~Client(void);
		Client(const Client &cp);
		Client &operator=(const Client &cp);

		// SETTERS:
		void setSocket(int socket);
		void setRequest(std::string request);
		void setFinishedRequest(bool finishedReq);

		// GETTERS:
		int getSocket(void);
		std::string getRequest(void);
		bool isFinishedRequest(void);

		// METHODS:
		void receiveData(void);

		// EXCEPTIONS:
		class RecvErrorException;
};

#endif
