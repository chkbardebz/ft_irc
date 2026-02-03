#include "../includes/server.hpp"

bool pass(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, password, more;

    if (!(ss >> cmd >> password))
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    if (more.empty() == false)
        return (write(client_fd, "461 ERR_NEEDLESSPARAMS\n", 24), false);
    if (strcmp(password.c_str(), serverDetails.getPass().c_str()) == 0)
    {
        std::map<int, Client>::iterator it = huntrill.find(client_fd);
        if (it->second.getStatusPass() == true)
            return (write(client_fd, "462 ERR_ALREADYREGISTERED\n", 27), false);
        it->second.setStatusPass(true);
    }
    else 
        return (write(client_fd, "464 ERR_PASSWDMISMATCH\n", 24), false); //? pas officiel selon la doc
    return(welcome_client(huntrill, client_fd), true);
}
