#include "../includes/server.hpp"

bool privmsg(int client_fd, std::string line, Server &serverDetails)
{
    if (is_already_registered(serverDetails, client_fd) == false)
        return (false);

    std::stringstream ss(line);
    std::string cmd, dest, first, message;

    ss >> cmd >> dest >> first; //! a mettre dans un if (!()) ??
    std::getline(ss, message); //! verifie si echoue ?
    if (dest[0] == ':')
        return (send_err_msg(serverDetails, client_fd, 411, ":No recipient given", NOT_INITIALIZED), false);
    if (first.empty() || (first[0] == ':' && first.size() == 1 && is_full_of_space(message, 0) == true))
        return (send_err_msg(serverDetails, client_fd, 412, ":No text to send", NOT_INITIALIZED), false);
    if (first[0] != ':' && first.size() >= 1)
        return (send_err_msg(serverDetails, client_fd, 407, ":Too many targets", dest + ' ' + first), false);
    if (dest[0] == '#')
        return (send_msg_to_channel(serverDetails, cmd, first + message, client_fd, dest), true);
    return (send_msg_to_client(serverDetails, client_fd, nick_to_fd(serverDetails, dest), cmd, first + ' ' + message), true);
}
