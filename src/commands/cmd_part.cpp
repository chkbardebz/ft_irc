#include "../../includes/server.hpp"

//? tous les chemins de channels doivent etre essayes mm si un (plus tot) renvoi un msg d'erreur.
//? PART ==> err PARAMS.
//? PART #<channel(s)> ==> quitte les channels sans laisser de msg.
//? PART #<channel(s)> <message> ==> quitte les channels en laissant un msg.
//? PART #general #random #help ==> INCORRECT
//? PART #general,#random,#help ==> VALID
bool part(int client_fd, std::string line, Server &serverDetails)
{
    if (is_already_registered(serverDetails, client_fd) == false)
        return (false);

    std::stringstream ss(line);
    std::string cmd, channel_s_unsplited, message = "";
    bool message_printable = false;

    // PART est forcement la car parsing mais channel a echoue donc il est absent ==> err
    if (!(ss >> cmd >> channel_s_unsplited))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    std::vector<std::string> channels_splited = ft_sukuna(channel_s_unsplited, ',');
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
            send_err_msg(serverDetails, client_fd, 476, ":Bad Channel Mask", NOT_INITIALIZED);
            continue ;
        }
        std::map<std::string,Channel>::iterator channel_details = serverDetails.makala.find(*channels_splited_it);
        // VERIFIE SI LE CHANNEL EXISTE
        if (channel_details == serverDetails.makala.end())
        {
            send_err_msg(serverDetails, client_fd, 403, ":No such channel", *channels_splited_it);
            continue ;
        }
        // VERIFIE SI LE CLIENT EST PRESENT SUR LE CHANNEL
        if(channel_details->second.is_fd_in_channel(client_fd) == false)
        {
            send_err_msg(serverDetails, client_fd, 442, ":You're not on that channel", NOT_INITIALIZED);
            continue ;
        }
        // VERIFIE S'IL Y A UN MSG A ENVOYR (FALCULTATIF)
        send_msg_to_channel(serverDetails, "PART", message, client_fd, *channels_splited_it);
        // FAIRE QUITTER LE CLIENT DU SERVER
        channel_details->second.client_quit_channel(client_fd);
        // FERME LE CHANNEL SI PLUS AUCUN CLIENT DESSUS
        if (channel_details->second.is_there_clients() == 0)
            serverDetails.makala.erase(channel_details);
    }
    return (clear_vector_sukuned(channels_splited), true);
}
