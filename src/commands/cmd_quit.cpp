#include "../../includes/server.hpp"

bool send_quit_to_peers(Server &serverDetails, int client_fd, std::string msg)
{
    std::set<int> channelFds; // iterator des fds du channel
    std::set<int> fdsToSendTo; // iterator des fds a qui envoyer un msg et pour eviter de leur en envoyer plusieurs

    for (std::map<std::string, Channel>::iterator it_chan = serverDetails.makala.begin(); it_chan != serverDetails.makala.end();)
    {
        if (it_chan->second.is_fd_in_channel(client_fd) == true)
        {
            channelFds = it_chan->second.getFds();
            for (std::set<int>::iterator it_set = channelFds.begin(); it_set != channelFds.end() ;it_set++)
            {
                if (*it_set == client_fd)
                    continue;
                if (fdsToSendTo.find(*it_set) != fdsToSendTo.end())
                    continue;
                fdsToSendTo.insert(*it_set);
            }
            it_chan->second.client_quit_channel(client_fd); //? Part de tous les channels rejoins
            if (it_chan->second.is_there_clients() == 0)
            {
                std::map<std::string, Channel>::iterator to_erase = it_chan;
                ++it_chan;
                serverDetails.makala.erase(to_erase);
                continue;
            }
        }
        ++it_chan;
    }
    for (std::set<int>::iterator it_toSend = fdsToSendTo.begin(); it_toSend != fdsToSendTo.end(); ++it_toSend)
        send(*it_toSend, msg.c_str(), msg.size(), 0);
    return (true);
}

bool quit(int client_fd, std::string line, Server &serverDetails)
{
    std::string msg;
    std::map<int,Client>::iterator it_hunt = serverDetails.huntrill.find(client_fd);
    if (it_hunt == serverDetails.huntrill.end())
        return (false);
    std::string str(line);
    if (!str.empty() && str[str.size()-1] == '\n' && str[str.size() - 2] == '\r')
        msg = ":" + it_hunt->second.getNick() + "!" + it_hunt->second.getUser() + "@localhost " + str;
    else
        msg = ":" + it_hunt->second.getNick() + "!" + it_hunt->second.getUser() + "@localhost " + str + "\r\n";
    send_quit_to_peers(serverDetails, client_fd, msg);
    it_hunt->second.resetClient();

    serverDetails.huntrill.erase(it_hunt);

    return (true);
}
// SI LE CLIENT QUITTE BRUTALEMENT (ordi s'eteint, ctrl-C etc.)
// QUIT :Ping timeout
// QUIT :Read error

