#include "includes/Conf.hpp"

int main()
{
	

    Conf myConf;

	std::cout << "|" << myConf.getHost() << "|\n";
    //std::cout << "|" << myConf.getPorts() << "|\n";
    std::cout << "|" << myConf.getName() << "|\n";
    return 0;
}