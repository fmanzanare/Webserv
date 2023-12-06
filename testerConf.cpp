#include "includes/Conf.hpp"
#include "includes/Route.hpp"
#include "includes/Server.hpp"
#include "includes/Client.hpp"

int main()
{
	Conf c("conf.yml");
	std::vector<Server *> servers = c.getServers();
	for (int i = 0; i < (int)servers.size(); i++)
	{
		std::cout <<  std::endl << std::endl << "Name: " << servers[i]->getName() << std::endl;
		std::cout << "ErrorPage: " << servers[i]->getErrPage() << std::endl;
		std::cout << "BodyLimit: " << servers[i]->getCBodyLimit() << std::endl;
		std::cout << "Host: " << servers[i]->getHost() << std::endl;
		std::cout << "Ports: \t";
		std::vector<int> ports = servers[i]->getPorts();
		for (int j = 0; j < (int)ports.size(); j++)
			std::cout << ports[j] << "\t";
		std::vector<Route *> routes = servers[i]->getRoutes();
		for (int j = 0; j < (int)routes.size(); j++)
		{
			std::cout << std::endl << "Routes: " << std::endl << std::endl;
			std::cout << "\tMethods: ";
			std::vector<std::string> methods = routes[j]->getAllowdMethods();
			for (int k= 0; k < (int)methods.size(); k++)
				std::cout << "\t" << methods[k] << "\t";
			std::cout << std::endl << "\tDirList: \t" << routes[j]->isDirListing() << std::endl;
			std::cout << "\tDefaultAnswer: \t" << routes[j]->getDefaultAnswer() << std::endl;
			std::cout << "\tCgi: \t";
			std::vector<std::string> cgis = routes[j]->getCgi();
			for (int k= 0; k < (int)cgis.size(); k++)
				std::cout << "\t" << cgis[k] << "\t";
			std::cout << std::endl << "\tRedirection: \t" << routes[j]->getRedir() << std::endl;
			std::cout << "\tRoot: \t\t" << routes[j]->getRoot() << std::endl;
		}
	}

}