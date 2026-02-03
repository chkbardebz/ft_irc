// void Channel::send_msg_to_channel(std::string message, std::map<int, Client> &huntrill, int client_fd)
// {
//     std::map<int, Client>::iterator it_hunt = huntrill.find(client_fd);
//     for (std::set<int>::iterator it = fds_channel.begin(); it != fds_channel.end(); it++)
//     {
//         std::string tmp = "";
//         if (client_fd == *it)
//             continue;
//         tmp = ":" + it_hunt->second.getNick() + "!" + it_hunt->second.getUser() + "@localhost " + "PRIVMSG " + _name + " " + message + '\n'; //! localhost ptetpas
//         send(*it, tmp.c_str(), tmp.size(), 0);
//     }
// }

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
