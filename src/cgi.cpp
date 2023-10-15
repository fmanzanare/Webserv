#include "../includes/cgi.hpp"

std::string cgi(std::string path)
{
	int status;

	if (access(path.c_str(), F_OK | R_OK) == -1)
		return("Onde??");

	//hard_code
	char *argv[3];
	argv[0] = (char *)path.c_str();
	argv[1] = (char *)"./s1/cgi/holamundo.py";
	argv[2] = 0;

	//ERROR 500?

	//creo el archivo temporal
	int temp = open(".temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
	//ERROR TIMEOUT 504?
	int pid = fork();

	if (!pid)
	{
		if (dup2(temp, STDOUT_FILENO) == -1)
			printf("Error al abrir el pipe");
			close(temp);
		execve(path.c_str(), argv, NULL);
		return ("error");
	}
	waitpid(pid, &status, 0);
	temp = open(".temp.txt", O_RDONLY);
	char buf[50];
	std::string data;
	ssize_t len = -1;
	while (len)
	{
		len = read(temp, buf, 50);
		data += std::string(buf, len);
	}
	return (data);
}

int main()
{
	std::cout << cgi("/usr/local/bin/python3") << std::endl;
}