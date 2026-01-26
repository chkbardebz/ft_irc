#include "../includes/ircserv.hpp"

bool pass(std::map<int, Client> &huntrill, int client_fd, char* line, Server serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, password, more;

    if (!(ss >> cmd >> password))
    {
        write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 23);
        return (false);
    }
    if (more.empty() == false)
    {
        write(client_fd, "vaffanffanffancyyyyy\n", 22); //!
        return (false);
    }

    if (strcmp(password.c_str(), serverDetails._password.c_str()) == 0)
    {
        std::map<int, Client>::iterator it = huntrill.find(client_fd);
        if (it->second.pass_is_set == true)
        {
            write(client_fd, "462 ERR_ALREADYREGISTERED\n", 27);
            return (false);
        }
        write(0, "ma biiiiiiite tu peux te brosser ma biiiiiiiitch\n", 50);
        it->second.pass_is_set = true;
    }
    else 
    {
        write(client_fd, "464 ERR_PASSWDMISMATCH\n", 24); //? pas officiel selon la doc
        return (false);
    }
    return(true);
}
