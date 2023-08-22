#ifndef WEBSERVS_HPP
#define WEBSERVS_HPP

#include "Server.hpp"
#include <poll.h>
#include <sstream>

#define MAX_CONNECTIONS 1000

class WebServs {
	private:
		// ATTRIBUTES:
		std::vector<Server *> _cluster;
		std::vector<int> _wSockets;
		std::vector<int> _cSockets;

		// METHODS:
		int addSocketsToPoll(pollfd *fds);
		void getServersSockets(void);
		void checkServersSockets(pollfd *fds, int nfds);

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
};

#endif
