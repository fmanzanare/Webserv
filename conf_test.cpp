#include "includes/Conf.hpp"

int main()
{
	

    Conf myConf;

	std::cout << "|" << myConf.getHost() << "|\n";
    std::cout << "|" << myConf.getPort() << "|\n";
    std::cout << "|" << myConf.getServerName() << "|\n";
    return 0;
}