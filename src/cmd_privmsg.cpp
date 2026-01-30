#include "../includes/server.hpp"

bool privmsg(std::map<int, Client> &huntrill, int client_fd, char *line, Server &serverDetails)
{
    (void)serverDetails;
    std::stringstream ss(line);
    std::string cmd, client, first ,message;
    std::string sendernick, senderuser ,recievernick;

    std::map<int, Client>::iterator it = huntrill.find(client_fd);
    if (is_client_set(huntrill, client_fd) == false)
        return (write(client_fd, "451 ERR_NOTREGISTERED\n", 23), false);
    ss >> cmd >> client >> first; 
    //! if ou pas pour ss ?????? 
    std::getline(ss, message);
    if (client[0] == '#')
    {
        std::map<std::string, Channel>::iterator it_channel = serverDetails.makala.begin();
        for (; it_channel != serverDetails.makala.end() ; it_channel++)
        {
            if (strcmp(it_channel->first.c_str(), client.c_str()) == 0)
            {
                if (it_channel->second.is_fd_in_channel(client_fd) == false)
                    return (write(client_fd, "999 ERR_CANNOTSENDTOCHAN\n", 26), false); //? client hors du chann
                it_channel->second.send_msg(first + message, huntrill, client_fd);
                return (true);
            }
        }
        return (write(client_fd, "999 ERR_NOSUCHCHANNEL\n", 23), false);
    }
        // return (false); //! Handle channels a rajouter // ERR_CANNOTSENDTOCHAN a gerer
    if (client[0] == ':')
        return (write(client_fd, "411 ERR_NORECIPIENT\n", 20), false);
    if (first.empty())
        return (write(client_fd, "412 ERR_NOTEXTTOSEND\n", 21), false);
    if (first[0] != ':' && first.size() >= 1)
        return (write(client_fd, "407 ERR_TOOMANYTARGETS\n", 23), false);
    if ((first[0] == ':'  && first.size() == 1) && check_if_fullofsapce(message) == true)
        return (write(client_fd, "412 ERR_NOTEXTTOSEND\n", 21), false);
    senderuser = it->second.getUser();
    sendernick = it->second.getNick();
    it = huntrill.begin();
    while (it != huntrill.end())
    {
        if (strcmp(it->second.getNick().c_str(), client.c_str()) == 0)
        {
            if (is_client_set(huntrill, it->first) == false)
                break;
            recievernick = it->second.getNick();
            message = ":" + sendernick + "!" + senderuser + "@localhost " + cmd + " " + recievernick + " " + first + message + '\n';
            send(it->first, message.c_str(), message.size(), 0);
            return (true);
        }
        it++;
    }
    return (write(client_fd, "401 ERR_NOSUCHNICK\n", 20), false);
}
