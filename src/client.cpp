#include "../includes/client.hpp"

Client::Client() : _nickname(NO_NICKNAME), _username(NO_USERNAME), _realname(NO_REALNAME), pass_is_set(false)
{
    // std::cout << "Client created" << std::endl;
}

Client::Client(std::string user, std::string nick, std::string realname) : _nickname(user), _username(nick), _realname(realname), pass_is_set(false)
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
    }
    return (*this);
}

Client::~Client()
{
    // std::cout << "Client destructor called" << std::endl;
}

std::string Client::getUser()
{
    return (_username);
}

std::string Client::getNick()
{
    return (_nickname);
}


std::string Client::getReal()
{
    return (_realname);
}


void Client::setUser(std::string username)
{
    _username = username;
}
void Client::setNick( std::string nickname)
{
    _nickname = nickname;
}

void Client::setReal( std::string realname)
{
    _realname = realname;
}



