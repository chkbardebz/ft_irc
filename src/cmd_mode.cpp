#include "../includes/server.hpp"

int count_words(const std::string& str)
{
    int count = 0;

    for (size_t i = 0; i < str.size(); i++)
    {
        if (!std::isspace(str[i]) &&
            (i == 0 || std::isspace(str[i - 1])))
        {
            count++;
        }
    }
    return count;
}

bool get_channel_mode(Server &serverDetails, std::map<int, Client> &huntrill, int client_fd, std::string channel)
{
    std::stringstream args;
    std::string modes;
    std::map<std::string, Channel>::iterator it_chan = serverDetails.makala.find(channel);

    if (it_chan == serverDetails.makala.end())
        return (write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23), false); // return ?
    if (it_chan->second.is_fd_op(client_fd) == false)
        return (write(client_fd, "482 ERR_CHANOPRIVSNEEDED\n", 26), false);
    std::map<int,Client>::iterator it_client = huntrill.find(client_fd);
    modes = " +";
    if (it_chan->second.getTopicStatus() == true)
        modes += "t";
    if (it_chan->second.getInviteOnlyStatus() == true)
        modes += "i";
    if (it_chan->second.getPasswordStatus() == true)
    {
        modes += "k";
        args << it_chan->second.getChanPassword() + " ";
    }
    if (it_chan->second.getUserLimitStatus() == true)
    {
        modes += "l";
        args << it_chan->second.getUserLimit();
    }
    std::string str = ":ircserv.local 324 " + it_client->second.getNick() + " " + channel + modes + " " + args.str() + "\r\n";
    send(client_fd, str.c_str(), str.size(), 0);
    return (true);
}

bool mode(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, channel, modes, args;

    if (!(ss>>cmd>>channel>>modes))
    {
        if (channel.empty())
            return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
        return (get_channel_mode(serverDetails, huntrill, client_fd, channel));
    }
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    if (it == serverDetails.makala.end())
        return (write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23), false);
    if (it->second.is_fd_op(client_fd) == false)
        return (write(client_fd, "482 ERR_CHANOPRIVSNEEDED\n", 26), false);
    std::getline(ss, args);
    if (it->second.is_fd_in_channel(client_fd) == false)
        return (write(client_fd, "442 ERR_NOTONCHANNEL\n", 22), false);
    int add = -1;
    int nb_args = 0;
    for (int i = 0; modes[i]; i++)
    {
        if (modes[i] == 'o' || modes[i] == 'l' || modes[i] == 'k')
            nb_args++;
    }
    std::stringstream arg_parsed(args);
    std::string arg;
    for (int i = 0; modes[i]; i++)
    {
        if (modes[i] == '+')
            add = true;
        if (modes[i] == '-')
            add = false;
        if (modes[i] == 'i') // i INVITEMODEONLY //! A revoir lorsque INVITE sera implemente
            it->second.setMode('i', add, NULL);
        if (modes[i] == 't') // t TOPICRESTIRCTED
            it->second.setMode('t', add, NULL);
        if (modes[i] == 'k') // k PASSWDCHANNEL
        {
            arg_parsed>>arg;
            if (arg.empty() && add == true)
                return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
            // std::cout << "MODE PASSWORD = |" << arg << "|" << std::endl;
            it->second.setMode('k', add, arg.c_str());
        }
        if (modes[i] == 'o') // o OPERCLIENT
        {
            arg_parsed>>arg;
            if (arg.empty())
                return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
            std::map<int, Client>::iterator it_hunt = huntrill.begin();
            for (; it_hunt != huntrill.end(); it_hunt++)
            {
                if (it->second.is_fd_in_channel(it_hunt->first) == true && strcmp(it_hunt->second.getNick().c_str(), arg.c_str()) == 0)
                {
                    if (it->second.is_fd_op(it_hunt->first) == true && add == true)
                        break;
                    if (it->second.is_fd_op(it_hunt->first) == false && add == false)
                        break;
                    if (it->second.is_fd_op(it_hunt->first) == true && add == false)
                    {
                        it->second.removeOp(it_hunt->first);
                        break;
                    }
                    else
                    {
                        it->second.setOp(it_hunt->first);
                        break;
                    }
                }
            }
            if (it_hunt == huntrill.end())
                return (write(client_fd, "441 ERR_USERNOTINCHANNEL\n", 26), false);
        }
        if (modes[i] == 'l') // l MAXCLIENTLIMIT
        {
            arg_parsed>>arg;
            if (arg.empty() && add == true)
                return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
            it->second.setMode('l', add, arg.c_str());
        }
    }
    return (send_cmd_broadcast(serverDetails, huntrill, "MODE", modes + arg_parsed.str(), client_fd, it->first), true);
}
