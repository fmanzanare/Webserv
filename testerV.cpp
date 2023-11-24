#include "includes/Response.hpp"
#include <dirent.h>
#include <string>
#include <iostream>
#include <filesystem>

int main(void)
{
	char path[] = "./s1/storage";
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(path)) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
			printf("%s\n", ent->d_name);
		closedir(dir);
	} 
	else 
	{
		perror("");
		return EXIT_FAILURE;
	}
}