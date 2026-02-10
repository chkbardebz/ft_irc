#include "../../includes/server.hpp"

bool pass(int client_fd, std::string line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, password, more;

    if (!(ss >> cmd >> password))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    // if (ss >> more)
    //     return (write(client_fd, "461 ERR_NEEDLESSPARAMS\n", 24), false);
    if (strcmp(password.c_str(), serverDetails.getPass().c_str()) == 0)
    {
        std::map<int, Client>::iterator it = serverDetails.huntrill.find(client_fd);
        if (it->second.getStatusPass() == true)
            return (send_err_msg(serverDetails, client_fd, 462, ":You may not reregister", NOT_INITIALIZED), false);
        it->second.setStatusPass(true);
    }
    else 
        return (send_err_msg(serverDetails, client_fd, 464, ":Password incorrect", NOT_INITIALIZED), false);
        
    return(welcome_client(serverDetails, client_fd), true);
}
