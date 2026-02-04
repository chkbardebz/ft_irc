#include "../includes/server.hpp"

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

void ft_putstr_fd(const char *str, int fd)
{
    for (int i = 0; str[i]; i++)
        write(fd , &str[i], 1);
}

bool is_client_set(std::map<int, Client> &huntrill, int client_fd)
{
    std::map<int, Client>::iterator it = huntrill.find(client_fd);

    if (it->second.getStatusNick() == true && it->second.getStatusPass() == true && it->second.getStatusUser() == true)
        return (true);
    return (false);
}

void welcome_client(std::map<int, Client> &huntrill, int client_fd)
{
    std::map<int, Client>::iterator it = huntrill.find(client_fd);

    if (it->second.getStatusNick() == true && it->second.getStatusPass() == true && it->second.getStatusUser() == true)
    {
        std::string ret_message = "001 "+ it->second.getNick() + " :Welcome to the IRC Network\n";
        ft_putstr_fd(ret_message.c_str(), it->first);
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
void send_err_msg(std::map<int, Client> &huntrill, int client_fd, int type_err, std::string reason, std::string message)
{
    std::string msg;
    std::ostringstream oss;
    oss << type_err;
    std::string type_err_s = oss.str();
    std::map<int, Client>::iterator it = huntrill.find(client_fd);
    if (reason == NOT_INITIALIZED)
        msg = ":ft_irc " + type_err_s + ' ' + it->second.getNick() + ' ' + message + '\n';
    else
        msg = ":ft_irc " + type_err_s + ' ' + it->second.getNick() + ' ' + reason + ' ' + message + '\n';
    send(client_fd, msg.c_str(), msg.size(), 0);
}


bool send_msg_to_channel(Server &serverDetails, std::map<int, Client> &huntrill, std::string cmd, std::string message, int sender_fd, std::string channel)
{
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    std::set<int>::iterator receiver_fd = it->second.getFds().begin();
    std::map<int, Client>::iterator it_sender = huntrill.find(sender_fd);
    //! secu find ?

    if (it->second.is_fd_in_channel(sender_fd) == false)
        return (write(sender_fd, "442 ERR_NOTONCHANNEL\n", 22), false);
    for (; receiver_fd != it->second.getFds().end(); receiver_fd++)
    {
        if (*receiver_fd == sender_fd)
            continue;
        std::string msg = ":" + it_sender->second.getNick() + "!" + it_sender->second.getUser() + "@localhost " + cmd + " " + channel + " " + message + '\n';
        send(*receiver_fd, msg.c_str(), msg.size(), 0);
    }
    return (true);
}

bool send_msg_to_client(std::map<int, Client> &huntrill, int sender_fd, int receiver_fd, std::string cmd, std::string message)
{
    std::map<int, Client>::iterator it_sender = huntrill.find(sender_fd);
    std::map<int, Client>::iterator it_receiver = huntrill.find(receiver_fd);
    //! secu find ?

    std::string msg = ":" + it_sender->second.getNick() + "!" + it_sender->second.getUser() + "@localhost " + cmd + " " + it_receiver->second.getNick() + " " + message + '\n';
    send(receiver_fd, msg.c_str(), msg.size(), 0);
    return (true);
}

bool send_cmd_broadcast(Server &serverDetails, std::map<int, Client> &huntrill, std::string cmd, std::string message, int sender_fd, std::string channel)
{
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    std::set<int>::iterator receiver_fd = it->second.getFds().begin();
    std::map<int, Client>::iterator it_sender = huntrill.find(sender_fd);

    for (; receiver_fd != it->second.getFds().end(); receiver_fd++)
    {
        std::string msg = ":" + it_sender->second.getNick() + "!" + it_sender->second.getUser() + "@localhost " + cmd + " " + channel + " " + message + '\n';
        send(*receiver_fd, msg.c_str(), msg.size(), 0);
    }
    return (true);
}

int nick_to_fd(std::map<int, Client> &huntrill, std::string nick)
{
    std::map<int, Client>::iterator it = huntrill.begin();
    for (; it != huntrill.end(); it++)
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
