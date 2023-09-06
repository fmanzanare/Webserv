#ifndef WEBSERVS_HPP
#define WEBSERVS_HPP

#include "Server.hpp"
#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include "Client.hpp"

#define MAX_CONNECTIONS 1000

class WebServs {
	private:
		// ATTRIBUTES:
		std::vector<Server *> _cluster;
		std::vector<int> _wSockets;
		std::vector<Client *> _clients;
		/* ------TO BE REMOVED------ */
		std::vector<int> _cSockets;
		/* ------TO BE REMOVED------ */
		int _nfds;

		// METHODS:
		void addSocketsToPoll(pollfd *fds);
		void getServersSockets(void);
		void checkServersSockets(pollfd *fds);
		void checkClientsSockets(pollfd *fds);
		void acceptPollinConnection(int inFd, int serverIdx);

	public:
		// ORTHODOOX CANNONICAL FORM:
		WebServs();
		WebServs(std::vector<Server *> cluster);
		~WebServs();
		WebServs(const WebServs &cp);
		WebServs &operator=(const WebServs &cp);

		// GETTERS:
		std::vector<Server *> getCluster(void);

		// SETTERS:
		void addServer(Server *s);
		void runWebServs(void);

		// EXCEPTIONS:
		class PollErrorException;
		class AcceptConnectionErrorException;
		class FcntlErrorException;
};

#endif
