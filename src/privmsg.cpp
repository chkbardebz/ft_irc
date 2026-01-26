#include "../includes/ircserv.hpp"

bool check_if_fullofsapce(std::string str)
{
    for(int i = 0; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (false);
    }
    return (true);
}

bool privmsg(std::map<int, Client> &huntrill, int client_fd, char *line, Server serverDetails)
{
    (void)serverDetails;
    std::stringstream ss(line);
    std::string cmd, client, first ,message;
    std::string sendernick, senderuser ,recievernick;

    ss >> cmd >> client >> first;
    if (client[0] == '#')
        return (false); //! Handle channels a rajouter // ERR_CANNOTSENDTOCHAN a gerer
    if (client[0] == ':')
    {
        write(client_fd, "411 ERR_NORECIPIENT\n", 20); //! PAS de destinataire
        return (false);
    }
    if (first.empty())
    {
        write(client_fd, "412 ERR_NOTEXTTOSEND\n", 21);
        return (false);
    }
    std::getline(ss, message);
    std::cout << "message len : " << first.size() << std::endl;
    if (first[0] != ':' && first.size() >= 1)
    {
        write(client_fd, "407 ERR_TOOMANYTARGETS\n", 23);
        return (false);
    }
    if ((first[0] == ':'  && first.size() == 1) && check_if_fullofsapce(message) == true)
    {
        write(client_fd, "412 ERR_NOTEXTTOSEND\n", 21);
        return (false);
    }
    std::map<int, Client>::iterator it = huntrill.find(client_fd);
    if(it != huntrill.end())
    {
        senderuser = it->second.getUser();
        sendernick = it->second.getNick();
    }
    it = huntrill.begin();
    while (it != huntrill.end())
    {
        if (strcmp(it->second.getNick().c_str(), client.c_str()) == 0)
        {
            recievernick = it->second.getNick();
            // message = message.substr(1);
            message = ":" + sendernick + "!" + senderuser + "@localhost " + cmd + " " + recievernick + " " + first + message + '\n'; //! remplacer user avec USER
            send(it->first, message.c_str(), message.size(), 0);
            return (true);
        }
        it++;
    }
    write(client_fd, "401 ERR_NOSUCHNICK\n", 21);
    return (false);
    
}

// int main(int ac, char **av)
// {
// char line[] = "PRIVMSG Jules :Salut l'equipe";

//     std::stringstream ss(line);
//     std::string cmd, client, message;


//     ss >> cmd >> client;
//     std::getline(ss, message);
//     if (message[1] != ':')
//         return (false);
//     message = message.substr(1);
//     std::cout << message << std::endl;
// }