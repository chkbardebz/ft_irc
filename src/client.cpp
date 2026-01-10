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

// Sets the user in operator mode so it cans use op commands
int Client::setOp(void)
{
    if (isoperator == true)
        std::cout << "user : " << _username << " is already an operator" << std::endl;
    else {
        isoperator = true;
        std::cout << "user : " << _username << " is now an operator" << std::endl;
    }
}

std::string Client::getUser()
{
    return _username;
}

std::string Client::getNick()
{
    return _nickname;
}