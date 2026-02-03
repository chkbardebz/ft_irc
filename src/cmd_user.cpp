#include "../includes/server.hpp"

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
    if (realname.empty() || realname[0] != ':' || is_full_of_space(realname, 1) == true)
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    realname.erase(0, 1); //? supp le ':'
    it->second.setUser(username);
    it->second.setStatusUser(true);
    it->second.setReal(realname);
    return(welcome_client(huntrill, client_fd), true);
}
