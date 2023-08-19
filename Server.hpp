#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

class Server {
	private:
		std::string					_name;
		std::vector<int>			_ports;
		std::vector<int>			_socks;
		std::string					_errPage;
		int							_cBodyLimit;
		std::vector<std::string>	_methods;
		bool						_dirListing;
		std::string					_defResponse;
		std::string					_redir;
		std::string					_servRoute;
	public:
		// ORTHODOX CANNONICAL FORM:
		Server();
		Server(std::vector<int> ports, std::vector<std::string> methods);
		~Server();
		Server(const Server &cp);
		Server &operator=(const Server &cp);

		// SETTERS:
		void setName(std::string name);
		void addPort(int port);
		void addSocket(int sock);
		void setErrPage(std::string errPage);
		void setCBodyLimit(int cBodyLimit);
		void addMethod(std::string method);
		void setDirListing(bool dirListing);
		void setDefResponse(std::string defResponse);
		void setRedir(std::string redir);
		void setServRoute(std::string servRoute);

		// GETTERS:
		std::string getName(void);
		std::vector<int> getPorts(void);
		std::vector<int> getSockets(void);
		std::string getErrPage(void);
		int getCBodyLimit(void);
		std::vector<std::string> getMethods(void);
		bool hasDirListing(void);
		std::string getDefResponse(void);
		std::string getRedir(void);
		std::string getServRoute(void);
};

#endif
