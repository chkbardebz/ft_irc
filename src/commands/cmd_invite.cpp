#include "../../includes/server.hpp"

bool invite(int client_fd, std::string line, Server &serverDetails)
{
    if (is_already_registered(serverDetails, client_fd) == false)
        return (false);

    std::stringstream ss(line);
    std::string cmd, nick, channel;

    if (!(ss>>cmd>>nick>>channel))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    std::map<int, Client>::iterator it_hunt = serverDetails.huntrill.begin();
    for (;it_hunt != serverDetails.huntrill.end(); it_hunt++)
    {
        if (it_hunt->second.getNick() == nick)
            break;
    }
    if (it_hunt == serverDetails.huntrill.end())
        return (send_err_msg(serverDetails, client_fd, 401, ":No such nick/channel", NOT_INITIALIZED), false);
    std::map<std::string, Channel>::iterator it_chan = serverDetails.makala.find(channel);
    if (it_chan != serverDetails.makala.end())
    {
        if (it_chan->second.getInviteOnlyStatus() == true && it_chan->second.is_fd_op(client_fd) == false)
            return (send_err_msg(serverDetails, client_fd, 428, ":You're not channel operator", NOT_INITIALIZED), false);
        if (it_chan->second.is_fd_in_channel(client_fd) == false && it_chan != serverDetails.makala.end()) //? si le client qui appelle la cmd n'est pas dans le channel et que le channel existe
            return (send_err_msg(serverDetails, client_fd, 442, ":You're not on that channel", NOT_INITIALIZED), false);    
        if (it_chan->second.is_fd_in_channel(it_hunt->first) == true && it_chan != serverDetails.makala.end())
            return (send_err_msg(serverDetails, client_fd, 443, ":Is already on channel", NOT_INITIALIZED), false);   
        it_chan->second.invite_fd(it_hunt->first);
        send_msg_to_client(serverDetails, client_fd, it_hunt->first, "INVITE ", channel);
    }
    return (true);
}
