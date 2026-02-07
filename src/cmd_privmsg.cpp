#include "../includes/server.hpp"

//? 1/ cherche le channel correspondant passe en params
//? 2/ envoi le msg a tous les client present dans le channel grace a send_msg()
// bool send_msg_to_channel(Server &serverDetails, std::map<int, Client> &huntrill, int client_fd, std::string channel, std::string message)
// {
//     std::map<std::string, Channel>::iterator it_channel = serverDetails.makala.begin();
//     for (; it_channel != serverDetails.makala.end() ; it_channel++)
//     {
//         if (strcmp(it_channel->first.c_str(), channel.c_str()) == 0)
//         {
//             if (it_channel->second.is_fd_in_channel(client_fd) == false)
//                 return (write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23), false); //? client hors du chann
//             it_channel->second.send_msg_to_channel(message, huntrill, client_fd);
//             return (true);
//         }
//     }
//     return (write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23), false);
// }

// bool send_msg_to_client_with_nick(std::map<int, Client> &huntrill, int client_fd, std::string cmd, std::string dest, std::string message)
// {
//     std::string sendernick, senderuser ,recievernick;
//     std::map<int, Client>::iterator it = huntrill.find(client_fd);

//     senderuser = it->second.getUser();
//     sendernick = it->second.getNick();

//     for (it = huntrill.begin() ; it != huntrill.end() ; it++)
//     {
//         if (strcmp(it->second.getNick().c_str(), dest.c_str()) == 0)
//         {
//             if (is_client_set(huntrill, it->first) == false)
//                 break;
//             recievernick = it->second.getNick();
//             message = ":" + sendernick + "!" + senderuser + "@localhost " + cmd + " " + recievernick + " " + message + '\n';
//             send(it->first, message.c_str(), message.size(), 0);
//             return (true);
//         }
//     }
//     return (write(client_fd, "401 ERR_NOSUCHNICK\n", 20), false);
// }

bool privmsg(std::map<int, Client> &huntrill, int client_fd, char *line, Server &serverDetails)
{
    if (is_already_registered(huntrill, client_fd) == false)
        return (false);

    std::stringstream ss(line);
    std::string cmd, dest, first, message;

    if (is_client_set(huntrill, client_fd) == false)
        return (write(client_fd, "451 ERR_NOTREGISTERED\n", 23), false);
    ss >> cmd >> dest >> first; //! a mettre dans un if (!()) ??
    std::getline(ss, message); //! verifie si echoue ?
    if (dest[0] == '#')
        return (send_msg_to_channel(serverDetails, huntrill, cmd, first + message, client_fd, dest), true);
    if (dest[0] == ':')
        return (write(client_fd, "411 ERR_NORECIPIENT\n", 20), false);
    if (first.empty())
        return (write(client_fd, "412 ERR_NOTEXTTOSEND\n", 21), false);
    if (first[0] != ':' && first.size() >= 1)
        return (write(client_fd, "407 ERR_TOOMANYTARGETS\n", 23), false);
    if ((first[0] == ':'  && first.size() == 1) && is_full_of_space(message, 0) == true)
        return (write(client_fd, "412 ERR_NOTEXTTOSEND\n", 21), false);
    return (send_msg_to_client(huntrill, client_fd, nick_to_fd(huntrill, dest), cmd, first + message), true);
}
