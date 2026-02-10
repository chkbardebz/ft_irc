#include "../../includes/server.hpp"

void name_reply(Server &serverDetails, int client_fd, std::string channel)
{
    std::map<std::string, Channel>::iterator it_chan = serverDetails.makala.find(channel);
    std::map<int, Client>::iterator it_client = serverDetails.huntrill.find(client_fd);

    std::string names, str, topicString;

    if (it_chan->second.getTopic() == NO_TOPIC)
        topicString = ":ircserv.local 331 " + it_client->second.getNick() + " " + channel + " " + it_chan->second.getTopic() + "\n"; //? RPL_NOTOPIC
    else
        topicString = ":ircserv.local 332 " + it_client->second.getNick() + " " + channel + " " + it_chan->second.getTopic() + "\n"; //? RPL_TOPIC
    send(client_fd, topicString.c_str(), topicString.size(), 0);
 
    for (std::set<int>::iterator it_fd = it_chan->second.getFds().begin(); it_fd != it_chan->second.getFds().end(); it_fd++)
    {
        it_client = serverDetails.huntrill.find(*it_fd);
        if (it_chan->second.is_fd_op(*it_fd) == true)
            names += "@" + it_client->second.getNick() + " ";
        else
            names += it_client->second.getNick() + " ";
    }
    it_client =  serverDetails.huntrill.find(client_fd);
    str = ":ircserv.local 353 " + it_client->second.getNick() + " " + channel + " :" + names + "\r\n";
    send(client_fd, str.c_str(), str.size(), 0);
    str = ":ircserv.local 366 " + it_client->second.getNick() + " " + channel + " :End of /NAMES list" + "\r\n";
    send(client_fd, str.c_str(), str.size(), 0);
}

bool join(int client_fd, std::string line, Server &serverDetails)
{
    if (is_already_registered(serverDetails, client_fd) == false)
        return (false);
        
    std::stringstream ss(line);
    std::string cmd, name, passwd;

    if (!(ss >> cmd >> name))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    std::map<std::string, Channel>::iterator it = serverDetails.makala.begin();
    if (name[0] != '#')
        return (send_err_msg(serverDetails, client_fd, 476, ":Bad Channel Mask", name), false);
    std::getline(ss, passwd);
    for (; it != serverDetails.makala.end(); it++)
    {
        if (strcmp(name.c_str(), it->second.getName().c_str()) == 0)
        {
            if (it->second.getUserLimitStatus() == true && it->second.getFds().size() + 1 > (size_t)it->second.getUserLimit())
                return (send_err_msg(serverDetails, client_fd, 471, ":Cannot join channel (+l)", name), false);
            if (it->second.getPasswordStatus() == true)
            {
                std::stringstream ss_passwd(passwd);
                if (!(ss_passwd>>passwd))
                    return (send_err_msg(serverDetails, client_fd, 475, ":Cannot join channel (+k)", name), false);
                if (passwd != it->second.getChanPassword())
                    return (send_err_msg(serverDetails, client_fd, 475, ":Cannot join channel (+k)", name), false);
            }
            if (it->second.getInviteOnlyStatus() == true && it->second.is_fd_invited(client_fd) == true)
                it->second.remove_invited(client_fd);
            else if (it->second.getInviteOnlyStatus() == true && it->second.is_fd_invited(client_fd) == false)
                return (send_err_msg(serverDetails, client_fd, 473, ":Cannot join channel (+i)", name), false);
            it->second.set_new_fd(client_fd);
            send_cmd_broadcast(serverDetails, "JOIN", "", client_fd, it->first);
            name_reply(serverDetails, client_fd, it->first);
            return (true);
        }
    }
    Channel new_class(name);
    serverDetails.makala.insert(std::make_pair(name, new_class));
    it = serverDetails.makala.find(name);
    it->second.set_new_fd(client_fd);
    send_cmd_broadcast(serverDetails, "JOIN", "", client_fd, it->first);
    name_reply(serverDetails, client_fd, name);
    return (true);
}