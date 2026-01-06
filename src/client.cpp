#include "client.hpp"

Client::Client() : _username("default"), _nickname("dft"), isoperator(false)
{
    // std::cout << "Client created" << std::endl;
}

Client::Client(std::string user, std::string nick, bool op) : _username(user), _nickname(nick), isoperator(op)
{
    // std::cout << nick << " Client created" << std::endl;
}

Client::Client(const Client &src)
{
    *this = src;
}

Client &Client::operator=(const Client &copy)
{
    if (this != &copy)
    {
        this->_username = copy._username;
        this->_nickname = copy._nickname;
        this->isoperator = copy.isoperator;
    }
    return (*this);
}

Client::~Client()
{
    // std::cout << "Client destructor called" << std::endl;
}