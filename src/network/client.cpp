#include "../../includes/server.hpp"

// == CONSTRUCTORS =========================================================================================================

//? UTILE std::map<int, Client> 
Client::Client() : _nickname(NOT_INITIALIZED), _username(NOT_INITIALIZED), _realname(NOT_INITIALIZED), _status_nick(false), _status_user(false), _status_pass(false), recvBuffer(NOT_INITIALIZED)
{
}

//? UTILE std::map
Client::Client(const Client &src) : _nickname(src._nickname), _username(src._username), _realname(src._realname), _status_nick(src._status_nick), _status_user(src._status_user), _status_pass(src._status_pass), recvBuffer(NOT_INITIALIZED)
{
}

//? UTILE std::map
Client &Client::operator=(const Client &copy)
{
    if (this != &copy)
    {
        _nickname = copy._nickname;
        _username = copy._username;
        _realname = copy._realname;
        _status_nick = copy._status_nick;
        _status_user = copy._status_user;
        _status_pass = copy._status_pass;
    }
    return (*this);
}

Client::~Client()
{
}

// == GETTERS =========================================================================================================

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

// == SETTERS =========================================================================================================

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
void Client::setStatusNick(bool new_status)
{
    _status_nick = new_status;
}

void Client::setStatusPass(bool new_status)
{
    _status_pass = new_status;
}

void Client::resetClient()
{
    setUser(NOT_INITIALIZED);
    setNick(NOT_INITIALIZED);
    setReal(NOT_INITIALIZED);

    setStatusNick(false);
    setStatusUser(false);
    setStatusPass(false);
}
