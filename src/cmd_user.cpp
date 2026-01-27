#include "../includes/server.hpp"

void ft_putstr_fd(const char *str, int fd)
{
    for (int i = 0; str[i]; i++)
        write(fd , &str[i], 1);
}

bool fullisspace(std::string str, int i)
{
    for(; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (false);
    }
    return (true);
}

bool is_client_set(std::map<int, Client> &huntrill, int client_fd)
{
    std::map<int, Client>::iterator it = huntrill.find(client_fd);

    if (it->second.getStatusNick() == true && it->second.getStatusPass() == true && it->second.getStatusUser() == true)
        return (true);
    return (false);
}

void is_client_welcome(std::map<int, Client> &huntrill, int client_fd)
{
    std::map<int, Client>::iterator it = huntrill.find(client_fd);

    if (it->second.getStatusNick() == true && it->second.getStatusPass() == true && it->second.getStatusUser() == true)
    {
        std::string ret_message = "001 "+ it->second.getNick() + " :Welcome to the IRC Network\n";
        ft_putstr_fd(ret_message.c_str(), it->first);
    }
}


bool user(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    (void)serverDetails;
    std::map<int, Client>::iterator it = huntrill.find(client_fd);
    if (it->second.getStatusUser() == true)
        return (write(client_fd, "462 ERR_ALREADYREGISTRED\n", 26), false);
    std::stringstream ss(line);
    std::string cmd, username, hostname, servname, realname;
    if (!(ss >> cmd >> username >> hostname >> servname))
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    getline(ss, realname);
    while (realname[0] && isspace(realname[0]))
        realname.erase(0, 1);
    if (realname.empty() || realname[0] != ':' || fullisspace(realname, 2) == true)
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    realname.erase(0, 1); //? supp le ':'
    it->second.setUser(username);
    it->second.setStatusUser(true);
    it->second.setReal(realname);
    return(is_client_welcome(huntrill, client_fd), true);
}
