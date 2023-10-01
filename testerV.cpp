#include "../includes/Response.hpp"

int main(void)
{
	std::vector<std::string> splitted = splitFilePath("/ihey.html");
	int i = 0;

	if (splitted.empty())
		return 0;
	else
	{
		while((unsigned long)i < splitted.size())
		{
			std::cout << "split " << i << ": " << splitted[i] << std::endl;
			i++;
		}
	}
}