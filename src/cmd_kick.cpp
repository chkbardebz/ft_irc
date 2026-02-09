#include "../includes/server.hpp" 

// ERR POSSIBLE :
//? => pas op pour kick => 482 ERR_CHANOPRIVSNEEDED ==> client_fd recoit => ":server 482 <nick> #chan :You're not channel operator"
//? => channel inexistant => 403 ERR_NOSUCHCHANNEL
//? => le user a kick n'existe pas => 401 ERR_NOSUCHNICK
//? => user pas dans le canal => 441 ERR_USERNOTINCHANNEL
//? => le kicker n'est pas dans le bon canal => 442 ERR_NOTONCHANNEL

bool kick(int client_fd, std::string line, Server &serverDetails)
{
    if (is_already_registered(serverDetails, client_fd) == false)
        return (false);

    std::stringstream ss(line);
    std::string cmd, channel_unsplited, user_unsplited, message = "";
    bool message_printable = false;

    if (!(ss >> cmd >> channel_unsplited >> user_unsplited))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    std::vector<std::string> channel_splited = ft_sukuna(channel_unsplited, ',');
    std::vector<std::string> user_splited = ft_sukuna(user_unsplited, ',');
    if (ss >> message)
        message_printable = true;
    if (message_printable == true && message[0] == ':')
    {
        std::string rest;
        std::getline(ss, rest);
        message += rest;
    }
    std::vector<std::string>::iterator it_channel_splited = channel_splited.begin();
    std::vector<std::string>::iterator it_user_splited = user_splited.begin();
    for (; it_channel_splited != channel_splited.end() && it_user_splited != user_splited.end() ;
     it_channel_splited++, it_user_splited++)
    {
        if (it_channel_splited->empty() || (*it_channel_splited)[0] != '#')
        {
            send_err_msg(serverDetails, client_fd, 476, ":Bad Channel Mask", NOT_INITIALIZED); //! pas dans rfc2812 askip
            continue ;
        }
        std::map<std::string,Channel>::iterator channel_details = serverDetails.makala.find(*it_channel_splited);
        // VERIFIE SI LE CHANNEL EXISTE
        if (channel_details == serverDetails.makala.end())
        {
            send_err_msg(serverDetails, client_fd, 403, ":No such channel", NOT_INITIALIZED);
            continue ;
        }
        // VERIFIE SI LE KICKEUR EST PRESENT SUR LE CHANNEL
        if(channel_details->second.is_fd_in_channel(client_fd) == false)
        {
            send_err_msg(serverDetails, client_fd, 442, ":You're not on that channel", NOT_INITIALIZED);
            continue ;
        }
        // VERIFIE SI LE KICKEUR EST OP
        if (channel_details->second.is_fd_op(client_fd) == false)
        {
            send_err_msg(serverDetails, client_fd, 482, ":You're not channel operator", NOT_INITIALIZED);
            continue ;
        }
        // VERIFIE SI LE NICK DE LA VICTIME EXISTE 
        int fd_kicked = nick_to_fd(serverDetails, *it_user_splited);
        if(fd_kicked < 0)
        {
            send_err_msg(serverDetails, client_fd, 401, ":No such nick/channel", NOT_INITIALIZED);
            continue ;
        }
        // VERIFIE SI LA VICTIME EST PRESENT SUR LE CHANNEL
        if(channel_details->second.is_fd_in_channel(fd_kicked) == false)
        {
            send_err_msg(serverDetails, client_fd, 441, ":They aren't on that channel", NOT_INITIALIZED); //! normalement precise le nick mais glr a repecher
            continue ;
        }
        // VERIFIE S'IL Y A UN MSG A ENVOYER (FALCULTATIF)
        if (send_msg_to_channel(serverDetails, cmd, *it_user_splited + " " + message, client_fd, *it_channel_splited) == false)
        {
            send_err_msg(serverDetails, client_fd, 403, ":No such channel", NOT_INITIALIZED);
            continue ;
        }
        // ENVOI LE BON MESSAGE SI AUTO KICK
        if (fd_kicked == client_fd)
            send_msg_to_client(serverDetails, client_fd, client_fd, cmd, message);
        // FAIRE QUITTER LE CLIENT DU CHANNEL
        channel_details->second.client_quit_channel(fd_kicked);
        // FERME LE CHANNEL SI PLUS AUCUN CLIENT DESSUS
        if (channel_details->second.is_there_clients() == 0)
            serverDetails.makala.erase(channel_details);
    }
    return (clear_vector_sukuned(channel_splited), clear_vector_sukuned(user_splited), true);
}
