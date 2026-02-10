#include "../../includes/server.hpp"

bool nick_check_duplicate(Server &serverDetails, std::string nickname)
{
    for (std::map<int, Client>::iterator it = serverDetails.huntrill.begin() ; it != serverDetails.huntrill.end() ; it++)
    {
        if ((std::strcmp(it->second.getNick().c_str(), nickname.c_str())) == 0)
            return(false);
    }
    return(true);
}

// nick peut se renommer pas user
bool nick(int client_fd, std::string line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, nickname; //, more
    std::map<int, Client>::iterator it = serverDetails.huntrill.find(client_fd);
    ss >> cmd >> nickname; // >> more
    if (nickname.empty()) //? err: pas de nickname fourni
        return (send_err_msg(serverDetails, client_fd, 431, ":No nickname given", NOT_INITIALIZED), false);
    if (nick_check_duplicate(serverDetails, nickname) == false) // err: nickname deja existant 
        return (send_err_msg(serverDetails, client_fd, 433, ":Nickname is already in use", nickname), false);
    if (std::isdigit(nickname[0]) || !is_valid_char(nickname, 1) || nickname.size() > 9) // err: first elem isdigit
        return (send_err_msg(serverDetails, client_fd, 432, ":Erroneous nickname", nickname), false);
    if (it != serverDetails.huntrill.end())  // Vérifier que le client existe
    {   
        it->second.setNick(nickname);
        if (it->second.getStatusNick() == false)
        {
            it->second.setStatusNick(true);
            welcome_client(serverDetails, client_fd);
        }
        return (true);
    }
    return(false);
}
