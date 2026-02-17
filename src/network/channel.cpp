#include "../../includes/server.hpp"

Channel::Channel(std::string name) : _name(name), _topic(":No topic is set"), _chan_password(":No password is set"), _max_user(0), _is_invite_only(false), _is_topic_restricted(false), _is_chan_password(false), _is_user_limit(false)
{
}

Channel::Channel(const Channel &copy)
{
    *this = copy;
}

Channel &Channel::operator=(const Channel &src)
{
    if (this != &src)
    {
        this->_name = src._name;
        this->fds_op = src.fds_op;
        this->_topic = src._topic;
        this->fds_channel = src.fds_channel;
        this->_chan_password = src._chan_password;
        this->_max_user = src._max_user;
        this->_is_invite_only = src._is_invite_only;
        this->_is_topic_restricted = src._is_topic_restricted;
        this->_is_chan_password = src._is_chan_password;
        this->_is_user_limit = src._is_user_limit;
    }
    return (*this);
}

Channel::~Channel()
{

}

const std::string &Channel::getName()
{
    return (_name);
}

const std::set<int>& Channel::getFds()
{
    return (fds_channel);
}

size_t Channel::getSize()
{
    return (fds_channel.size());
}

const std::string &Channel::getTopic()
{
    return (_topic);
}

void Channel::setTopic(std::string str)
{
    _topic = str;
}

void Channel::setMode(char mode, bool add, const char *arg)
{
    if (mode == 'i')
        _is_invite_only = add;
    if (mode == 't')
        _is_topic_restricted = add;
    if (mode == 'k')
    {
        _is_chan_password = add;
        setChanPassword(arg);
    }
    if (mode == 'l')
    {
        _is_user_limit = add;
        if (add == true)
            _max_user = std::atoi(arg);
        else
            _max_user = 0;
    }
}

void Channel::removeOp(int client_fd)
{
    std::set<int>::iterator it = fds_op.find(client_fd);
    fds_op.erase(*it);
}

void Channel::setOp(int client_fd)
{
    fds_op.insert(client_fd);
}

bool Channel::getUserLimitStatus()
{
    return (_is_user_limit);
}

int Channel::getUserLimit()
{
    return (_max_user);
}

bool Channel::getPasswordStatus()
{
    return _is_chan_password;
}

const std::string &Channel::getChanPassword()
{
    return _chan_password;
}

void Channel::setChanPassword(const std::string &str)
{
    _chan_password = str;
}

bool Channel::getInviteOnlyStatus()
{
    return (_is_invite_only);
}

void Channel::set_new_fd(int client_fd)
{
    if (getSize() == 0)
    {
        fds_channel.insert(client_fd);
        fds_op.insert(client_fd);
    }
    else
        fds_channel.insert(client_fd);
}

bool Channel::is_fd_in_channel(int client_fd)
{
    for (std::set<int>::iterator it = fds_channel.begin(); it != fds_channel.end(); it++)
    {
        if (*it == client_fd)
            return (true);
    }
    return (false);
}


bool Channel::client_quit_channel(int client_fd)
{
    // supprimer l'utilisateur des clients OP
    for (std::set<int>::iterator it = fds_op.begin(); it != fds_op.end(); it++)
    {
        if (*it == client_fd)
        {
            fds_op.erase(it);
            break ;
        }
    }
    // supprime l'utilisateur des clients
    for (std::set<int>::iterator it = fds_channel.begin(); it != fds_channel.end(); it++)
    {
        if (*it == client_fd)
        {
            fds_channel.erase(it);
            return (true);
        }
    }
    return (false);
}


bool Channel::is_fd_op(int client_fd)
{
    if (is_fd_in_channel(client_fd) == false)
        return (false);
    for (std::set<int>::iterator it = fds_op.begin(); it != fds_op.end(); it++)
    {
        if (*it == client_fd)
            return (true);
    }
    return (false);
}

bool Channel::is_there_clients()
{
    return (fds_channel.size());
}

bool Channel::is_fd_invited(int client_fd)
{
    for (std::set<int>::iterator it = fds_invited.begin(); it != fds_invited.end(); it++)
    {
        if (*it == client_fd)
            return (true);
    }
    return (false);
}

void Channel::invite_fd(int fd_invited)
{
    fds_invited.insert(fd_invited);
}

void Channel::remove_invited(int client_fd)
{
    fds_invited.erase(client_fd);
}

bool Channel::getTopicStatus()
{
    return (_is_topic_restricted);
}
