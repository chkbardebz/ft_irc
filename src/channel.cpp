#include "../includes/server.hpp"

Channel::Channel(std::string name) : _name(name), _topic(":No topic is set")
{

}

Channel::Channel(const Channel &copy)
{
    *this = copy;
}

Channel &Channel::operator=(const Channel &src) //! rajouter chaque membre de client au fur et a mesure
{
    if (this != &src)
    {
        this->_name = src._name;
        this->fds_op = src.fds_op;
        this->_topic = src._topic;
        this->fds_channel = src.fds_channel;
    }
    return (*this);
}

Channel::~Channel()
{

}

std::string Channel::getName()
{
    return (_name);
}

std::set<int> Channel::getFds()
{
    return (fds_channel);
}

size_t Channel::getSize()
{
    return (fds_channel.size());
}

std::string Channel::getTopic()
{
    return (_topic);
}

void Channel::setTopic(std::string str)
{
    _topic = str;
}

void Channel::send_msg(std::string message, std::map<int, Client> &huntrill, int client_fd)
{
    for (std::set<int>::iterator it = fds_channel.begin(); it != fds_channel.end(); it++)
    {
        std::string tmp = "";
        if (client_fd == *it)
            continue;
        std::map<int, Client>::iterator it_hunt = huntrill.find(*it);
        tmp = ":" + it_hunt->second.getNick() + "!" + it_hunt->second.getUser() + "@localhost " + "PRIVMSG " + _name + " " + message + '\n'; //! localhost ptetpas
        send(*it, tmp.c_str(), tmp.size(), 0);
    }
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