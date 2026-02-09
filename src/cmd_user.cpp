#include "../includes/server.hpp"

bool user(int client_fd, std::string line, Server &serverDetails)
{
    std::map<int, Client>::iterator it = serverDetails.huntrill.find(client_fd);
    if (it->second.getStatusUser() == true)
        return (send_err_msg(serverDetails, client_fd, 462, ":You may not reregister", NOT_INITIALIZED), false);
    std::stringstream ss(line);
    std::string cmd, username, hostname, servname, realname;
    if (!(ss >> cmd >> username >> hostname >> servname))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    getline(ss, realname);
    while (realname[0] && isspace(realname[0]))
        realname.erase(0, 1);
    if (realname.empty() || realname[0] != ':' || is_full_of_space(realname, 1) == true)
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    realname.erase(0, 1); //? supp le ':'
    it->second.setUser(username);
    it->second.setStatusUser(true);
    it->second.setReal(realname);
    return(welcome_client(serverDetails, client_fd), true);
}
