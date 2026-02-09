#include "../includes/server.hpp"

bool is_valid_port(char* str)
{
    for (size_t i = 0 ; str[i] ; i++)
    {
        if (std::isdigit(str[i]) == false)
            return (false);
    }
    return (true);
}

bool is_valid_char( const std::string str, int i )
{
    for (; str[i] ; i++)
    {
        if (std::isalpha(str[i]) == false && std::isdigit(str[i]) == false)
            return (false);
    }
    return (true);
}

bool is_full_of_space(std::string str, size_t i)
{
    for(; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (false);
    }
    return (true);
}

void welcome_client(Server &serverDetails, int client_fd)
{
    std::map<int, Client>::iterator it = serverDetails.huntrill.find(client_fd);

    if (it->second.getStatusNick() == true && it->second.getStatusPass() == true && it->second.getStatusUser() == true)
    {
        std::string ret_message = "001 "+ it->second.getNick() + " :Welcome to the IRC Network\r\n";
        send(it->first, ret_message.c_str(), ret_message.size(), 0);
    }
}

// void send_msg_to_client_with_fd(std::map<int, Client> &huntrill, std::string cmd, std::string message, int receiver_fd, int sender_fd)
// {
//     std::map<int, Client>::iterator it_send = huntrill.find(sender_fd);
//     std::map<int, Client>::iterator it_receive = huntrill.find(receiver_fd);
//     std::string msg = ":" + it_send->second.getNick() + "!" + it_send->second.getUser() + "@localhost " + cmd + it_receive->second.getNick() + " " + message + '\n';
//     send(receiver_fd, msg.c_str(), msg.size(), 0);
// }

// ===============================================================================================================

// :<server> <numeric> <target> [<params>] :<message>
void send_err_msg(Server &serverDetails, int client_fd, int type_err, std::string message, std::string reason)
{
    std::string msg;
    std::ostringstream oss;
    oss << type_err;
    std::string type_err_s = oss.str();
    std::map<int, Client>::iterator it = serverDetails.huntrill.find(client_fd);
    if (reason == NOT_INITIALIZED)
        msg = ":ircserv.local " + type_err_s + ' ' + it->second.getNick() + ' ' + message + "\r\n";
    else
        msg = ":ircserv.local " + type_err_s + ' ' + it->second.getNick() + ' ' + reason + ' ' + message + "\r\n";
    send(client_fd, msg.c_str(), msg.size(), 0);
}

bool send_msg_to_channel(Server &serverDetails, std::string cmd, std::string message, int sender_fd, std::string channel)
{
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    std::set<int>::iterator receiver_fd = it->second.getFds().begin();
    std::map<int, Client>::iterator it_sender = serverDetails.huntrill.find(sender_fd);
    //! secu find ?

    if (it == serverDetails.makala.end())
        return (send_err_msg(serverDetails, sender_fd, 403, ":No such channel", channel), false);
    if (it->second.is_fd_in_channel(sender_fd) == false)
        return (send_err_msg(serverDetails, sender_fd, 442, ":You're not on that channel", NOT_INITIALIZED), false);
    for (; receiver_fd != it->second.getFds().end(); receiver_fd++)
    {
        if (*receiver_fd == sender_fd)
            continue;
        std::string msg = ":" + it_sender->second.getNick() + "!" + it_sender->second.getUser() + "@localhost " + cmd + " " + channel + " " + message + "\r\n";
        send(*receiver_fd, msg.c_str(), msg.size(), 0);
    }
    return (true);
}

bool send_msg_to_client(Server &serverDetails, int sender_fd, int receiver_fd, std::string cmd, std::string message)
{
    std::map<int, Client>::iterator it_sender = serverDetails.huntrill.find(sender_fd);
    std::map<int, Client>::iterator it_receiver = serverDetails.huntrill.find(receiver_fd);
    //! secu find ?

    std::string msg = ":" + it_sender->second.getNick() + "!" + it_sender->second.getUser() + "@localhost " + cmd + " " + it_receiver->second.getNick() + " " + message + "\r\n";
    send(receiver_fd, msg.c_str(), msg.size(), 0);
    return (true);
}

bool send_cmd_broadcast(Server &serverDetails, std::string cmd, std::string message, int sender_fd, std::string channel)
{
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    std::set<int>::iterator receiver_fd = it->second.getFds().begin();
    std::map<int, Client>::iterator it_sender = serverDetails.huntrill.find(sender_fd);

    for (; receiver_fd != it->second.getFds().end(); receiver_fd++)
    {
        std::string msg = ":" + it_sender->second.getNick() + "!" + it_sender->second.getUser() + "@localhost " + cmd + " " + channel + " " + message + "\r\n";
        send(*receiver_fd, msg.c_str(), msg.size(), 0);
    }
    return (true);
}

int nick_to_fd(Server &serverDetails, std::string nick)
{
    std::map<int, Client>::iterator it = serverDetails.huntrill.begin();
    for (; it != serverDetails.huntrill.end(); it++)
    {
        if (it->second.getNick() == nick)
            return (it->first);
    }
    return (-1);
}



std::vector<std::string> ft_sukuna(std::string channel_s, char delim)
{
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = channel_s.find(delim, start)) != std::string::npos) 
    {
        result.push_back(channel_s.substr(start, end - start));
        start = end + 1;
    }
    result.push_back(channel_s.substr(start));
    return (result);
}

void clear_vector_sukuned(std::vector<std::string> &channels_splited)
{
    channels_splited.clear(); //? supprime tous les elements
    std::vector<std::string>().swap(channels_splited); //? libère la memoire allouee par le vector grace a un vector temporaire vide
}


// =====================================

bool is_already_registered(Server &serverDetails, int client_fd)
{
    std::string msg;
    std::map<int, Client>::iterator it = serverDetails.huntrill.find(client_fd);

    if (it->second.getStatusNick() == true && it->second.getStatusPass() == true && it->second.getStatusUser() == true)
        return(true);
    if (it->second.getStatusNick() == false)
        msg = ":ircserv.local 451 <nickname> :You have not registered\n";
    else
        msg = ":ircserv.local 451 " + it->second.getNick() + " :You have not registered\n";
    send(client_fd, msg.c_str(), msg.size(), 0);
    return(false);
}


// send_err_msg(huntrill, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 403, ":No such channel", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 401, ":No such nick/channel", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 428, ":You're not channel operator", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 442, ":You're not on that channel", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 443, ":Is already on channel", NOT_INITIALIZED)

// send_err_msg(huntrill, client_fd, 476, ":Bad Channel Mask", )
// send_err_msg(huntrill, client_fd, 471, ":Cannot join channel (+l)", )
// send_err_msg(huntrill, client_fd, 473, ":Cannot join channel (+i)", )
// send_err_msg(huntrill, client_fd, 482, ":You're not channel operator", NOT_INITIALIZED)


// send_err_msg(huntrill, client_fd, 441, ":They aren't on that channel", )


// send_err_msg(huntrill, client_fd, 431, ":No nickname given", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 432, ":Erroneous nickname", )
// send_err_msg(huntrill, client_fd, 433, ":Nickname is already in use", nickname)


// send_err_msg(huntrill, client_fd, 462, ":You may not reregister", NOT_INITIALIZED)
// send_err_msg(huntrill, client_fd, 464, ":Password incorrect", NOT_INITIALIZED)
