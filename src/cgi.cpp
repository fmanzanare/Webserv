#include "../includes/cgi.hpp"

std::string cgi(std::string path, char **env)
{
	int fd[2];
	int status;

	if (access(path.c_str(), F_OK | R_OK) == -1)
		return("Onde??");
	//hard_code
	char *argv[3];
	argv[0] = (char *)path.c_str();
	argv[1] = (char *)"./s1/cgi/holamundo.py";
	argv[2] = 0;
	//ERROR 500?
	if (pipe(fd) == -1)
		printf("Error al abrir el pipe");
	//ERROR TIMEOUT 504?

	//
	int pid = fork();
	if (!pid)
	{
		//ERROR 500?
		// if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
		// 	printf("Error al abrir el pipe");
		execve(path.c_str(), argv, NULL);
		return ("error");
	}
	// if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
	// 		printf("Error al abrir el pipe");

	// std::string line;
	// read(0, line, 1000)

	// std::getline(iss, line);
	// std::cout << line << std::endl;
	waitpid(pid, &status, 0);
	return ("fin");
}

int main(int ac, char **av, char **env)
{
	std::cout << cgi("/usr/local/bin/python3", env) << std::endl;
}