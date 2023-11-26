#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <vector>
#include <iostream>

class Route {
	private:
		// ATTRIBUTES:
		std::vector<std::string>	_methods;
		std::string					_redir;
		std::string					_root;
		bool						_dirListing;
		std::string					_default;
		// CGI.
		std::string					_cgi;
		// PRIVATE CONSTRUCTOR:
		Route();

	public:
		// ORTHODOX CANNONICAL FORM:
		Route(std::vector<std::string> methods, std::string redir, std::string root, bool dirListing, std::string def, std::string cgi);
		~Route();
		Route(const Route &cp);
		Route &operator=(const Route &cp);

		// SETTERS:
		void addMethod(std::string method);
		void setMethods(std::vector<std::string> methods);
		void setRedir(std::string redir);
		void setRoot(std::string root);
		void setDirListing(bool dirListing);
		void setDefaultAnswer(std::string def);
		void setCgi(std::string cgi);

		// GETTERS:
		std::vector<std::string> getAllowdMethods(void);
		std::string getRedir(void);
		std::string getRoot(void);
		bool isDirListing(void);
		std::string getDefaultAnswer(void);
		std::string getCgi(void);

		// METHODS:
		bool checkMethod(std::string method);
};

#endif
