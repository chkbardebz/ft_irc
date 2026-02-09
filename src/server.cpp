#include "../includes/server.hpp"

// == CONSTRUCTORS =========================================================================================================

//? UTILE
Server::Server() : _password(NOT_INITIALIZED), _res(NULL)
{
    std::memset(&_hints, 0, sizeof(_hints));
}

//? UTILE
Server::Server(std::string password) : _password(password), _res(NULL)
{
    std::memset(&_hints, 0, sizeof(_hints));
}

Server::~Server()
{
}

// == GETTERS =========================================================================================================

std::string Server::getPass()
{
    return (this->_password);
}

struct addrinfo& Server::getHints() 
{
    return (this->_hints);
}

struct addrinfo* Server::getRes() const
{
    return (this->_res);
}

//? Getter special ici pour int getaddrinfo(const char *__restrict__ __name, const char *__restrict__ __service, const addrinfo *__restrict__ __req, addrinfo **__restrict__ __pai)...
//? ...qui attend un ptr de ptr (struct addrinfo**) car elle alloue elle meme et modifie le ptr.
struct addrinfo** Server::getResAddr() 
{
    return (&this->_res);
}

struct pollfd *Server::getFds()
{
    return (this->_fds);
}

// == SETTERS =========================================================================================================

void Server::setPass(std::string password)
{
    this->_password = password;
}

void Server::setHints(const struct addrinfo& hints)
{
    this->_hints = hints;
}

void Server::setRes(struct addrinfo *res)
{
    this->_res = res;
}

void Server::setFds(struct pollfd *fds)
{
    this->_fds = fds;
}
