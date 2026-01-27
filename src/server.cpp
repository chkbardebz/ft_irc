#include "../includes/server.hpp"

Server::Server() : _password(NO_PASS)
{
    // std::cout << "Client created" << std::endl;
}

Server::Server(std::string password) : _password(password)
{
    // std::cout << nick << " Client created" << std::endl;
}
//!
// Server::Server( const Server &copy )
// {
//     *this = copy;
// }
//!
// Server &Server::operator=(const Server &copy)
// {
//     if (this != &copy)
//     {  
//         this->_password = copy._password;
//         this->_hints = copy._hints;
//         this->_res = copy._res;
//     }
//     return (*this);
// }

Server::~Server()
{
}

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

struct addrinfo** Server::getResAddr() //getter special pour int getaddrinfo(const char *__restrict__ __name, const char *__restrict__ __service, const addrinfo *__restrict__ __req, addrinfo **__restrict__ __pai) qui attend un ptr de ptr (struct addrinfo**) car elle alloue elle meme et modifie le ptr
{
    return (&this->_res);
}

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
