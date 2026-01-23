#include "../includes/client.hpp"

Client::Client() : _nickname("default_nickname"), _username("default_username"), _isoperator(false)
{
    // std::cout << "Client created" << std::endl;
}

Client::Client(std::string user, std::string nick, bool op) : _nickname(user), _username(nick), _isoperator(op)
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
        this->_isoperator = copy._isoperator;
    }
    return (*this);
}

Client::~Client()
{
    // std::cout << "Client destructor called" << std::endl;
}

// Sets the user in operator mode so it cans use op commands
void Client::setOp(void)
{
    if (_isoperator == true)
        std::cout << "user : " << _username << " is already an operator" << std::endl;
    else {
        _isoperator = true;
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

void Client::setUser(std::string user)
{
    _username = user;
}
void Client::setNick( std::string nick)
{
    _nickname = nick;
}