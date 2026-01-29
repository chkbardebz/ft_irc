#include "../includes/server.hpp"

bool check_if_fullofsapce(std::string str)
{
    for(int i = 0; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (false);
    }
    return (true);
}

bool topic(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, channel, topic;

    if (!(ss >> cmd >> channel))
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    getline(ss, topic);
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    if (it == serverDetails.makala.end())
        return (write(client_fd, "999 ERR_NOSUCHCHANNEL\n", 23), false);
    if (topic.empty() || check_if_fullofsapce(topic))
    {
        std::map<int,Client>::iterator it_hunt = huntrill.find(client_fd);
        std::string topicString = "331 " + it_hunt->second.getNick() + " " + channel + it->second.getTopic() + "\n";
        send(client_fd, topicString.c_str(), topicString.size(), 0);
        return (true);
    }
    else //! lorsque MODE sera fait mettre verif si les restrictions de TOPIC sont on/off
        it->second.setTopic(topic);
    return (true);
}
