#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

class Request
{
	public:
		// Constructors
		Request();
		Request(const Request &copy);
		
		// Destructor
		~Request();
		
		// Operators
		Request & operator=(const Request &assign);

		// Methods //
		std::string	processRequest(std::string);	

		// Setters
		void		setRawRequest(std::string);
		void		setMethod(std::string);
		void		setPath(std::string);
		void		setProtocol(std::string);
		void		setHost(std::string);

		// Getters
		std::string	getRawRequest(void);
		std::string	getMethod(void);
		std::string	getPath(void);
		std::string	getProtocol(void);
		std::string	getHost(void);
		std::string	getBody(void);

	private:
		std::string	_rawRequest;
		std::string	_method;
		std::string	_path;
		std::string	_protocol;
		std::string	_host;
		std::string	_body;
};

#endif