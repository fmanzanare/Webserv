#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.hpp"

class Server {
	private:
		// ATTRIBUTES:
		std::string					_name;
		std::vector<int>			_ports;
		std::string					_host;
		std::vector<int>			_socks;
		std::string					_errPage;
		int							_cBodyLimit;
		std::vector<std::string>	_methods;
		bool						_dirListing;
		std::string					_defResponse;
		std::string					_redir;
		std::string					_servRoute;
		std::vector<Client *>		_clients;

		// PRIVATE CONSTRUCTOR:
		Server();

		// PRIVATE METHODS:
		void openSockets(void);

	public:
		// ORTHODOX CANNONICAL FORM:
		Server(std::vector<int> ports, std::vector<std::string> methods, std::string host);
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
		void addMethod(std::string method);
		void setDirListing(bool dirListing);
		void setDefResponse(std::string defResponse);
		void setRedir(std::string redir);
		void setServRoute(std::string servRoute);
		void addClient(Client *client);
		void removeClient(int idx);

		// GETTERS:
		std::string getName(void);
		std::vector<int> getPorts(void);
		std::string getHost(void);
		std::vector<int> getSockets(void);
		std::string getErrPage(void);
		int getCBodyLimit(void);
		std::vector<std::string> getMethods(void);
		bool hasDirListing(void);
		std::string getDefResponse(void);
		std::string getRedir(void);
		std::string getServRoute(void);
		std::vector<Client *> getClients(void);

		// EXCEPTIONS:
		class SocketCreationErrorException;
		class SocketBindErrorException;
		class SocketListenErrorException;
};

#endif
