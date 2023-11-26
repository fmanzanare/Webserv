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
		bool			_finishedRes;
		bool			_errReadWrite;
		size_t			_resPos;

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
		void setFinishedResponse(bool finishedRes);

		// GETTERS:
		int getSocket(void);
		std::string getRequest(void);
		bool isFinishedRequest(void);
		bool isFinishedResponse(void);
		bool isErrorReadWrite(void);

		// OPERATORS:
		bool operator==(const Client client);

		// METHODS:
		void receiveData(void);
		void sendData(std::string response);

		// EXCEPTIONS:
		class RecvErrorException;
};

#endif
