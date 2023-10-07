#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <algorithm>
#include "Client.hpp"
#include "Route.hpp"

class Server {
	private:
		// ATTRIBUTES:
		std::string					_name;
		std::vector<int>			_ports;
		std::string					_host;
		std::vector<int>			_socks;
		std::string					_errPage;
		int							_cBodyLimit;
		std::vector<Route *>		_routes;
		std::vector<Client *>		_clients;

		// PRIVATE CONSTRUCTOR:
		Server();

		// PRIVATE METHODS:

	public:
		// ORTHODOX CANNONICAL FORM:
		Server(std::vector<int> ports, std::vector<std::string> methods, std::string host);//Deprecated
		Server(std::string name, std::vector<int> ports, std::string host, std::string errPage, int cBodyLimit, std::vector<Route *> routes);
		~Server();
		Server(const Server &cp);
		Server &operator=(const Server &cp);

		// SETTERS:
		void setName(std::string name);
		void addPort(int port);
		void setHost(std::string host);
		void addSocket(int sock);
		void setErrPage(std::string errPage);
		void setCBodyLimit(int cBodyLimit);
		void setRoutes(std::vector<Route *> routes);
		void addRoute(Route *route);
		void addClient(Client *client);
		void removeClient(Client *client);

		// GETTERS:
		std::string getName(void);
		std::vector<int> getPorts(void);
		std::string getHost(void);
		std::vector<int> getSockets(void);
		std::string getErrPage(void);
		int getCBodyLimit(void);
		std::vector<Route *> getRoutes(void);
		std::vector<Client *> getClients(void);

		// METHODS:
		void openSockets(void);

		// EXCEPTIONS:
		class SocketCreationErrorException;
		class SocketBindErrorException;
		class SocketListenErrorException;
};

#endif
