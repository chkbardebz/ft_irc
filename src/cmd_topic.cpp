#include "../includes/server.hpp"


bool topic(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails)
{
    if (is_already_registered(huntrill, client_fd) == false)
        return (false);
    std::stringstream ss(line);
    std::string cmd, channel, topic, topicString;

    if (!(ss >> cmd >> channel))
        return (write(client_fd, "461 ERR_NEEDMOREPARAMS\n", 24), false);
    getline(ss, topic);
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    if (it == serverDetails.makala.end())
        return (write(client_fd, "403 ERR_NOSUCHCHANNEL\n", 23), false);
    if (topic.empty() || is_full_of_space(topic, 0))
    {
        std::map<int,Client>::iterator it_hunt = huntrill.find(client_fd);
        if (it->second.getTopic() == NO_TOPIC)
            topicString = ":ircserv.local 331 " + it_hunt->second.getNick() + " " + channel + " " + it->second.getTopic() + "\n"; //? RPL_NOTOPIC
        else
            topicString = ":ircserv.local 332 " + it_hunt->second.getNick() + " " + channel + " " + it->second.getTopic() + "\n"; //? RPL_TOPIC
        send(client_fd, topicString.c_str(), topicString.size(), 0);
        return (true);
    }
    if (it->second.is_fd_op(client_fd) == false && it->second.getTopicStatus() == true)
        return (write(client_fd, "482 ERR_CHANOPRIVSNEEDED\n", 26), false);
    std::stringstream arg_parsed(topic);
    std::string first, rest;
    arg_parsed>>first;
    getline(arg_parsed, rest);
    if (first == ":" && (rest.empty() || is_full_of_space(rest, 0) == true))
    {
        it->second.setTopic(NO_TOPIC);
        return (true);
    }
    it->second.setTopic(first + rest);
    send_msg_to_channel(serverDetails, huntrill, "TOPIC", first + rest, client_fd, it->first);
    return (true);
}
