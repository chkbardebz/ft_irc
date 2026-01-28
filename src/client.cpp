#include "../includes/server.hpp"

Client::Client() : _nickname(NOT_INITIALIZED), _username(NOT_INITIALIZED), _realname(NOT_INITIALIZED), _status_nick(false), _status_user(false), _status_pass(false)
{
}

Client::Client(std::string user, std::string nick, std::string realname) : _nickname(user), _username(nick), _realname(realname), _status_nick(false), _status_user(false), _status_pass(false)
{
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
        this->_realname = copy._nickname;
        this->_status_nick = copy._status_nick; //!
        this->_status_user = copy._status_user; //!
        this->_status_pass = copy._status_pass; //!
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

bool Client::getStatusNick()
{
    return (_status_nick);
}

bool Client::getStatusUser()
{
    return (_status_user);
}


bool Client::getStatusPass()
{
    return (_status_pass);
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

void Client::setStatusUser(bool new_status)
{
    _status_user = new_status;
}
void Client::setStatusNIck(bool new_status)
{
    _status_nick = new_status;
}

void Client::setStatusPass(bool new_status)
{
    _status_pass = new_status;
}
