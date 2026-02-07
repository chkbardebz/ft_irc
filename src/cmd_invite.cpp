#include "../includes/server.hpp"

bool invite(std::map<int, Client> &huntrill, int client_fd, char *line, Server &serverDetails)
{
    if (is_already_registered(huntrill, client_fd) == false)
        return (false);

    std::stringstream ss(line);
    std::string cmd, nick, channel;

    if (!(ss>>cmd>>nick>>channel))
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    std::map<int, Client>::iterator it_hunt = huntrill.begin();
    for (;it_hunt != huntrill.end(); it_hunt++)
    {
        if (it_hunt->second.getNick() == nick)
            break;
    }
    if (it_hunt == huntrill.end())
        return(write(client_fd, "401 ERR_NOSUCHNICK\n", 20), false);
    std::map<std::string, Channel>::iterator it_chan = serverDetails.makala.find(channel);
    if (it_chan != serverDetails.makala.end())
    {
        if (it_chan->second.getInviteOnlyStatus() == true && it_chan->second.is_fd_op(client_fd) == false)
            return (write(client_fd, "482 ERR_CHANOPRIVSNEEDED\n", 26), false);
        if (it_chan->second.is_fd_in_channel(client_fd) == false && it_chan != serverDetails.makala.end()) //? si le client qui appelle la cmd n'est pas dans le channel et que le channel existe
            return (write(client_fd, "442 ERR_NOTONCHANNEL\n", 22), false);
        if (it_chan->second.is_fd_in_channel(it_hunt->first) == true && it_chan != serverDetails.makala.end())
            return (write(client_fd, "443 ERR_USERONCHANNEL\n", 23), false);
        it_chan->second.invite_fd(it_hunt->first); //! ret true
        send_msg_to_client(huntrill, client_fd, it_hunt->first, "INVITE ", channel);
    }
    //! else // fait rien lorsque le channel existe pas ?! mais ret true
    return (true);
}
