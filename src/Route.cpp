#include "../includes/Route.hpp"

// ORTHODOX CANNONICAL FORM:
Route::Route() {}

Route::Route(std::vector<std::string> methods, std::string redir, std::string root, bool dirListing, std::string def, std::string cgi) {
	for (int i = 0; i < (int)methods.size(); i++) {
		this->_methods.push_back(methods[i]);
	}
	this->_redir = redir;
	this->_root = root;
	this->_dirListing = dirListing;
	this->_default = def;
	this->_cgi = cgi;
}

Route::~Route() {}

Route::Route(const Route &cp) {
	for (int i = 0; i < (int)cp._methods.size(); i++) {
		this->_methods.push_back(cp._methods[i]);
	}
	this->_redir = cp._redir;
	this->_root = cp._root;
	this->_dirListing = cp._dirListing;
	this->_default = cp._default;
}

Route &Route::operator=(const Route &cp) {
	for (int i = 0; i < (int)cp._methods.size(); i++) {
		this->_methods.push_back(cp._methods[i]);
	}
	this->_redir = cp._redir;
	this->_root = cp._root;
	this->_dirListing = cp._dirListing;
	this->_default = cp._default;
	return (*this);
}

// SETTERS:
void Route::addMethod(std::string method) { this->_methods.push_back(method); }

void Route::setMethods(std::vector<std::string> methods) {
	for (int i = 0; i < (int)methods.size(); i++) {
		this->_methods.push_back(methods[i]);
	}
}

void Route::setRedir(std::string redir) { this->_redir = redir; }

void Route::setRoot(std::string root) { this->_root = root; }

void Route::setDirListing(bool dirListing) { this->_dirListing = dirListing; }

void Route::setDefaultAnswer(std::string def) { this->_default = def; }

void Route::setCgi(std::string cgi) { this->_cgi = cgi; }

// GETTERS:
std::vector<std::string> Route::getAllowdMethods() { return this->_methods; }

std::string Route::getRedir() { return this->_redir; }

std::string Route::getRoot() { return this->_root; }

bool Route::isDirListing() { return this->_dirListing; }

std::string Route::getDefaultAnswer() { return this->_default; }

std::string Route::getCgi() { return this->_cgi; }

// METHODS:
bool Route::checkMethod(std::string method) {
	if (std::find(this->_methods.begin(), this->_methods.end(), method) != this->_methods.end()) {
		return (true);
	}

	return (false);
}
