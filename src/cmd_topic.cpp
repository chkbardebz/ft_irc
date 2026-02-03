#include "../includes/server.hpp"


bool topic(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    std::stringstream ss(line);
    std::string cmd, channel, topic;

    if (!(ss >> cmd >> channel))
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    getline(ss, topic);
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    if (it == serverDetails.makala.end())
        return (write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23), false);
    if (topic.empty() || is_full_of_space(topic, 0))
    {
        std::map<int,Client>::iterator it_hunt = huntrill.find(client_fd);
        std::string topicString = "331 " + it_hunt->second.getNick() + " " + channel + it->second.getTopic() + "\n";
        send(client_fd, topicString.c_str(), topicString.size(), 0);
        return (true);
    }
    if (it->second.is_fd_op(client_fd) == false && it->second.getTopicStatus() == true)
        return (write(client_fd, "482 ERR_CHANOPRIVSNEEDED\n", 26), false);
    it->second.setTopic(topic);
    return (true);
}
