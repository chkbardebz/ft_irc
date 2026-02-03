#include "../includes/server.hpp"

bool nick_check_duplicate(std::map<int, Client>& huntrill, std::string nickname)
{
    for (std::map<int, Client>::iterator it = huntrill.begin() ; it != huntrill.end() ; it++)
    {
        if ((std::strcmp(it->second.getNick().c_str(), nickname.c_str())) == 0)
            return(false);
    }
    return(true);
}

// nick peut se renommer pas user
bool nick(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    
    (void)serverDetails;
    std::stringstream ss(line);
    std::string cmd, nickname; //, more
    std::map<int, Client>::iterator it = huntrill.find(client_fd);
    ss >> cmd >> nickname; // >> more
    if (nickname.empty()) //? err: pas de nickname fourni
        return (write(client_fd, "431 ERR_NONICKNAMEGIVEN\n", 21), false);
    if (nick_check_duplicate(huntrill, nickname) == false) // err: nickname deja existant 
        return (write(client_fd, "433 ERR_NICKNAMEINUSE\n", 23), false);
    if (std::isdigit(nickname[0]) || !is_valid_char(nickname, 1) || nickname.size() > 9) // err: first elem isdigit
        return (write(client_fd, "432 ERR_ERRONEUSNICKNAME\n", 26), false);
    if (it != huntrill.end())  // Vérifier que le client existe
    {
        it->second.setNick(nickname);
        it->second.setStatusNIck(true);
    }
    return(welcome_client(huntrill, client_fd), true);
}
