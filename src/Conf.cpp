#include "../includes/Conf.hpp"

class Conf::NoAllowPort : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Port\n");
	}
};

class Conf::NoAllowMethod : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Method\n");
	}
};

class Conf::NoAllowDirListing : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow DirListing\n");
	}
};

class Conf::NoAllowNameServer : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Name Server\n");
	}
};

class Conf::NoAllowHost : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Host\n");
	}
};

class Conf::NoAllowcBodyLimit : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Body Limit\n");
	}
};

class Conf::NoAllowRoot : public std::exception {
	virtual const char *what() const throw(){
		return ("No Allow Root\n");
	}
};

static std::string trim(std::string str)
{
	int i = 0;
	while (str[i] == ' ')
		i++;
	return str.substr(i);
}

void Conf::loadmap(std::map <std::string, std::string> m)
{
	std::map<std::string, std::string>::iterator it = m.begin();
			while (it != m.end())
			{
				if (!it->first.compare("methods"))
				{
					if (!this->_methods.empty())
						this->_methods.clear();
					setMethods(it->second);
				}
				if (!it->first.compare("directory-listing"))
					setDirListing(it->second);
				if (!it->first.compare("default-answer"))
					setDef(it->second);
				if (!it->first.compare("cgi"))
				{
					if (!this->_cgi.empty())
						this->_cgi.clear();
					setCgi(it->second);
				}
				if (!it->first.compare("redirection"))
					setRedir(it->second);
				if (!it->first.compare("root"))
					setRoot(it->second);
				if (!it->first.compare("server-name"))
					setName(it->second);
				if (!it->first.compare("error-page"))
					setError_page(it->second);
				if (!it->first.compare("body-limit"))
					setCBodyLimit(it->second);
				if (!it->first.compare("host"))
					setHost(it->second);
				if (!it->first.compare("port"))
				{
					if (!this->_ports.empty())
						this->_ports.clear();
					setPorts(it->second);
				}
				if (it != m.end())
					it++;
			}
}

Conf::Conf(std::string fileName)
{
	std::fstream archivo(fileName);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
    }
	std::string line;
	//Iteramos por los servers
	std::map<std::string, std::string> m;
	while(std::getline(archivo, line))
	{
		if (line.empty())
			continue ;
		const unsigned long pos = line.find(":");
		if (pos == std::string::npos)
			continue ;
		std::string key = trim(line.substr(0, pos));
		std::string value = trim(line.substr(pos + 1));
		if (key.compare("server") && key.compare("route"))
			m[key] = value;
		else if (!key.compare("route"))
		{
			loadmap(m);
			if (!this->_methods.empty() && !this->_def.empty() && !this->_redir.empty() && !this->_root.empty())
			{
				this->_routes.push_back(new Route(getMethods(), getRedir(), getRoot(), getDirListing(), getDef(), getCgi()));
				this->_methods.clear();
				this->_def.clear();
				this->_cgi.clear();
				this->_redir.clear();
				this->_root.clear();
			}
			m.clear();
		}
		else if (!key.compare("server"))
		{
			loadmap(m);
			if (!this->_methods.empty() && !this->_def.empty() && !this->_redir.empty() && !this->_root.empty())
			{
				this->_routes.push_back(new Route(getMethods(), getRedir(), getRoot(), getDirListing(), getDef(), getCgi()));
				this->_methods.clear();
				this->_def.clear();
				this->_cgi.clear();
				this->_redir.clear();
				this->_root.clear();
			}
			if (!this->_name.empty() && !this->_errPage.empty() && !this->_host.empty() && !this->_ports.empty() && !this->_routes.empty() && this->_cBodyLimit > 0)
			{
				this->_servers.push_back(new Server(getName(), getPorts(), getHost(), getError_page(), getCBodyLimit(), this->_routes));
				this->_name.clear();
				this->_errPage.clear();
				this->_host.clear();
				this->_routes.clear();
				this->_ports.clear();
				this->_cBodyLimit = -1;
			}
			m.clear();
		}
	}
	loadmap(m);
	if (!this->_methods.empty() && !this->_def.empty() && !this->_redir.empty() && !this->_root.empty())
	{
		this->_routes.push_back(new Route(getMethods(), getRedir(), getRoot(), getDirListing(), getDef(), getCgi()));
		this->_methods.clear();
		this->_def.clear();
		this->_cgi.clear();
		this->_redir.clear();
		this->_root.clear();
	}
	if (!this->_name.empty() && !this->_errPage.empty() && !this->_host.empty() && !this->_ports.empty() && !this->_routes.empty() && this->_cBodyLimit > 0)
	{
		this->_servers.push_back(new Server(getName(), getPorts(), getHost(), getError_page(), getCBodyLimit(), this->_routes));
		this->_name.clear();
		this->_errPage.clear();
		this->_host.clear();
		this->_routes.clear();
		this->_ports.clear();
		this->_methods.clear();
		this->_def.clear();
		this->_cgi.clear();
		this->_redir.clear();
		this->_root.clear();
		this->_cBodyLimit = -1;
		m.clear();
	}
	archivo.close();
}

Conf::~Conf()
{
}
//Methods
void	Conf::freeServer(void)
{
	for (int i = 0; i < (int)this->_servers.size(); i++)
			delete(this->_servers[i]);
	for (int i = 0; i < (int)this->_routes.size(); i++)
		delete(this->_routes[i]);
}
//SETs
void	Conf::setName(std::string name){
	for (int i = 0; i < (int)this->_servers.size(); i++)
		if (name == this->_servers[i]->getName())
		{
			freeServer();
			throw NoAllowNameServer();
		}
	this->_name = name;
}
void	Conf::setError_page(std::string errPage){
	if (errPage.front() != '/')
		errPage = "/" + errPage;
	this->_errPage = errPage;
}
void	Conf::setCBodyLimit(std::string cBodyLimit){
	char *aux = (char *)cBodyLimit.c_str();
	for (int i = 0; i < (int)cBodyLimit.size(); i++)
		{
			if (!std::isdigit(aux[i]))
			{
				freeServer();
				throw NoAllowcBodyLimit();
			}
		}
	int result = std::atoi(aux);
	if (0 > result)
	{
		freeServer();
		throw NoAllowcBodyLimit();
	}
	this->_cBodyLimit = result;
}
void	Conf::setHost(std::string host){
	this->_host = host;
}
void	Conf::setPorts(std::string ports){
	std::istringstream iss(ports);
	std::string			token;

	while (std::getline(iss, token, ','))
	{
		for (int i = 0; i < (int)token.size(); i++)
		{
			char *aux = (char *)token.c_str();
			if (!std::isdigit(aux[i]))
			{
				freeServer();
				throw NoAllowPort();
			}
		}
		int result = std::atoi(token.c_str());
		if (2000 >= result)
		{
			freeServer();
			throw NoAllowPort();
		}
		for (int i = 0; i < (int)this->_servers.size(); i++)
		{
			std::vector<int> ports = this->_servers[i]->getPorts();
			for (int j = 0; i < (int)ports[j]; j++)
				if (ports[j] == result)
				{
					freeServer();
					throw NoAllowPort();
				}
		}
		this->_ports.push_back(result);
	}
}

void	Conf::setMethods(std::string method){
	std::istringstream iss(method);
	std::string			token;

	int	nGet = 0;
	int	nPost = 0;
	int	nDelete = 0;
	while (std::getline(iss, token, ','))
	{
		if (token != "GET" && token != "POST" && token != "DELETE")
		{
			freeServer();
			throw NoAllowMethod();
		}
		if (token == "GET" && !nGet++)
			this->_methods.push_back(token);
		else if (token == "POST" && !nPost++)
			this->_methods.push_back(token);
		else if (token == "DELETE" && !nDelete++)
			this->_methods.push_back(token);
		else
		{
			freeServer();
			throw NoAllowMethod();
		}
	}
}

void	Conf::setDirListing(std::string dirListing){
	if (dirListing.find("true"))
		this->_dirListing = false;
	else if (dirListing.find("false"))
		this->_dirListing = true;
	else
	{
		freeServer();
		throw NoAllowDirListing();
	}
}
void	Conf::setDef(std::string def){
	this->_def = def;
}
void	Conf::setCgi(std::string cgi){
	std::istringstream iss(cgi);
	std::string			token;

	while (std::getline(iss, token, ','))
	{
		this->_cgi.push_back(token);
	}
}
void	Conf::setRedir(std::string redir){
	if (redir.back() != '/')
		redir = redir + '/';
	this->_redir = redir;
}
void	Conf::setRoot(std::string root){
	for (int i = 0; i < (int)this->_routes.size(); i++)
		if (this->_routes[i]->getRoot() == root)
		{
			freeServer();
			throw NoAllowRoot();
		}
	if (root[0] != '.')
		root = '.' + root;
	if (access(root.c_str(), F_OK) == -1)
	{
		std::cout << "acceso denegado" << std::endl;
		std::cout << "root: " << root << std::endl;
		freeServer();
		throw NoAllowRoot();
	}
	this->_root = root;
}

//GET
std::string					Conf::getName(void){
	return (this->_name);
}
std::string					Conf::getError_page(void){
	return (this->_errPage);
}
int							Conf::getCBodyLimit(void){
	return (this->_cBodyLimit);
}
std::string					Conf::getHost(void){
	return (this->_host);
}
std::vector<int>			Conf::getPorts(void){
	return (this->_ports);
}

std::vector<Route *>		Conf::getRoutes(void){
	return (this->_routes);
}

std::vector<std::string>	Conf::getMethods(void){
	return (this->_methods);
}
bool						Conf::getDirListing(void){
	return (this->_dirListing);
}
std::string					Conf::getDef(void){
	return (this->_def);
}
std::vector<std::string>	Conf::getCgi(void){
	return (this->_cgi);
}
std::string					Conf::getRedir(void){
	return (this->_redir);
}
std::string					Conf::getRoot(void){
	return (this->_root);
}

std::vector<Server *>		Conf::getServers(void){
	return (this->_servers);
}
