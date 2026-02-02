#include "../includes/server.hpp"

#include <vector>


bool fullisspace2(std::string str, size_t i)
{
    for(; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (false);
    }
    return (true);
}

std::vector<std::string> split_channel_s(std::string channel_s, char delim)
{
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = channel_s.find(delim, start)) != std::string::npos) 
    {
        result.push_back(channel_s.substr(start, end - start));
        start = end + 1;
    }
    result.push_back(channel_s.substr(start));
    return (result);
}

void clear_channels_splited(std::vector<std::string> &channels_splited)
{
    channels_splited.clear(); //? supprime tous les elements
    std::vector<std::string>().swap(channels_splited); //? libère la memoire allouee par le vector grace a un vector temporaire vide
}

//? tous les chemins de channels doivent etre essayes mm si un (plus tot) renvoi un msg d'erreur.
//? PART ==> err PARAMS.
//? PART #<channel(s)> ==> quitte les channels sans laisser de msg.
//? PART #<channel(s)> <message> ==> quitte les channels en laissant un msg.
//? PART #general #random #help ==> INCORRECT
//? PART #general,#random,#help ==> VALID
bool part(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, channel_s_unsplited, message = "";
    bool message_printable = false;

    // PART est forcement la car parsing mais channel a echoue donc il est absent ==> err
    if (!(ss >> cmd >> channel_s_unsplited))
        return (write(client_fd, "461 RR_NEEDMOREPARAMS\n", 23), false);
    std::vector<std::string> channels_splited = split_channel_s(channel_s_unsplited, ',');
    if (ss >> message)
        message_printable = true;
    // SI COMMENCE PAR ':' CELA INDIQUE QUE LE MESSAGE CONTIENT DES ESPACES DONC ON PRENDS TOUT AVEC GETLINE
    // SI NE COMMENCE PAS PAR ':' LE DERNIER ELEMENT RECRACHE PAR SS DEVIENT LE REASON/MESSAGE MM SI CE N'EST PAS LE CAS (pour etre en accord avec RFC2812)
    if (message_printable == true && message[0] == ':')
    {
        std::string rest;
        std::getline(ss, rest);
        message += rest;
    }
    for (std::vector<std::string>::iterator channels_splited_it = channels_splited.begin();
     channels_splited_it != channels_splited.end(); ++channels_splited_it) 
    {
        if (channels_splited_it->empty() || (*channels_splited_it)[0] != '#')
        {
            write(client_fd, "476 ERR_BADCHANMASK\n", 21);
            continue ;
        }
        std::map<std::string,Channel>::iterator channel_details = serverDetails.makala.find(*channels_splited_it);
        // VERIFIE SI LE CHANNEL EXISTE
        if (channel_details == serverDetails.makala.end())
        {
            write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23);
            continue ;
        }
        // VERIFIE SI LE CLIENT EST PRESENT SUR LE CHANNEL
        if(channel_details->second.is_fd_in_channel(client_fd) == false)
        {
            write(client_fd, "442 ERR_NOTONCHANNEL\n", 22);
            continue ;
        }
        // VERIFIE S'IL Y A UN MSG A ENVOYR (FALCULTATIF)
        if (send_msg_to_channel(serverDetails, huntrill, client_fd, *channels_splited_it, message) == false) //renvoi deja un msg d'err s'il faut
            continue ;
        // FAIRE QUITTER LE CLIENT DU SERVER
        channel_details->second.client_quit_channel(client_fd);
        // FERME LE CHANNEL SI PLUS AUCUN CLIENT DESSUS
        if (channel_details->second.is_there_clients() == 0)
            serverDetails.makala.erase(channel_details);
    }
    return (clear_channels_splited(channels_splited), true);
}

//! BEFORE CHANGEMENT DE LOGIQUE AVEC LE ':'
//! ':' n'indique pas le debut du reason/message MAIS precise uniquement si il y a des espaces dans celui-ci
// bool part(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
// {
//     std::stringstream ss(line);
//     std::string cmd, channel_s_unsplited, message;
//     bool message_printable = false;

//     if (!(ss >> cmd >> channel_s_unsplited)) // PART est forcement la car parsing mais channel a echoue donc il est absent ==> err
//         return (write(client_fd, "461 RR_NEEDMOREPARAMS\n", 23), false);

//     std::vector<std::string> channels_splited = split_channel_s(channel_s_unsplited, ',');
//     std::getline(ss, message);
//     while (message[0] && isspace(message[0]))
//         message.erase(0, 1);
//     if (message[0] == ':' && fullisspace2(message, 1) == false) //!
//         message_printable = true;
//     for (std::vector<std::string>::iterator channels_splited_it = channels_splited.begin();
//      channels_splited_it != channels_splited.end(); ++channels_splited_it) 
//     {
//         if (channels_splited_it->empty() || (*channels_splited_it)[0] != '#')
//         {
//             write(client_fd, "476 ERR_BADCHANMASK\n", 21);
//             continue ;
//         }
        
//         std::map<std::string,Channel>::iterator channel_details = serverDetails.makala.find(*channels_splited_it);
//         std::cout << "verifChannelSplited=" << *channels_splited_it << std::endl;

//         // VERIFIE SI LE CHANNEL EXISTE
//         if (channel_details == serverDetails.makala.end())
//         {
//             write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23);
//             continue ;
//         }
//         // VERIFIE SI LE CLIENT EST PRESENT SUR LE CHANNEL
//         if(channel_details->second.is_fd_in_channel(client_fd) == false)
//         {
//             write(client_fd, "442 ERR_NOTONCHANNEL\n", 22);
//             continue ;
//         }
//         // VERIFIE S'IL Y A UN MSG A ENVOYR (FALCULTATIF)
//         if (message_printable == true)
//         {
//             if (send_msg_to_channel(serverDetails, huntrill, client_fd, *channels_splited_it, message) == false) //renvoi deja un msg d'err s'il faut
//                 continue ;
//         }
//         // FAIRE QUITTER LE CLIENT DU SERVER
//         channel_details->second.client_quit_channel(client_fd);
//         // FERME LE CHANNEL SI PLUS AUCUN CLIENT DESSUS
//         if (channel_details->second.is_there_clients() == 0)
//             serverDetails.makala.erase(channel_details);
//     }
//     return (clear_channels_splited(channels_splited), true);
// }
