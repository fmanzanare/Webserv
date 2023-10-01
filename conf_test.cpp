#include "includes/Conf.hpp"
#include "includes/WebServs.hpp"
#include "includes/Request.hpp"
#include "includes/Response.hpp"
#include <csignal>

int main()
{
    try {
        Conf myConf;
        std::vector<Server *> Servers = myConf.getServers();
        std::vector<int> a = Servers[0]->getPorts();
        std::cout << a[1] << std::endl;
    } catch (...) {
        std::cout << "Wrong configuration!" << std::endl;
    }

    // std::cout << "Name=>|" << myConf.getName() << std::endl;
    // std::cout << "Error_page=>|" << myConf.getError_page() << std::endl;
    // std::cout << "CBodyLimit=>|" << myConf.getCBodyLimit() << std::endl;
	// std::cout << "Host=>|" << myConf.getHost() << std::endl;
    // // std::cout << "|" << myConf.getPorts() << "|\n";

    // //std::cout << "|" << myConf.getMethods() << "|\n";
    // std::cout << "DirListing=>|" << myConf.getDirListing() << std::endl;
    // std::cout << "Def=>|" << myConf.getDef() << std::endl;
    // std::cout << "Cgi=>|" << myConf.getCgi() << std::endl;
    // std::cout << "Redir=>|" << myConf.getRedir() << std::endl;
    // std::cout << "Root=>|" << myConf.getRoot() << std::endl;

    return 0;
}