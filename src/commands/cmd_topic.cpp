#include "../../includes/server.hpp"


bool topic(int client_fd, std::string line, Server &serverDetails)
{
    if (is_already_registered(serverDetails, client_fd) == false)
        return (false);
    std::stringstream ss(line);
    std::string cmd, channel, topic, topicString;

    if (!(ss >> cmd >> channel))
        return (send_err_msg(serverDetails, client_fd, 461, ":Not enough parameters", NOT_INITIALIZED), false);
    std::getline(ss, topic);
    std::map<std::string, Channel>::iterator it = serverDetails.makala.find(channel);
    if (it == serverDetails.makala.end())
        return (send_err_msg(serverDetails, client_fd, 403, ":No such channel", NOT_INITIALIZED), false);
    if (topic.empty() || is_full_of_space(topic, 0))
    {
        std::map<int,Client>::iterator it_hunt = serverDetails.huntrill.find(client_fd);
        if (it->second.getTopic() == NO_TOPIC)
            topicString = ":ircserv.local 331 " + it_hunt->second.getNick() + " " + channel + " " + it->second.getTopic() + "\n"; //? RPL_NOTOPIC
        else
            topicString = ":ircserv.local 332 " + it_hunt->second.getNick() + " " + channel + " " + it->second.getTopic() + "\n"; //? RPL_TOPIC
        send(client_fd, topicString.c_str(), topicString.size(), 0);
        return (true);
    }
    if (it->second.is_fd_op(client_fd) == false && it->second.getTopicStatus() == true)
        return (send_err_msg(serverDetails, client_fd, 482, ":You're not channel operator", NOT_INITIALIZED), false);
    std::stringstream arg_parsed(topic);
    std::string first, rest;
    arg_parsed>>first;
    std::getline(arg_parsed, rest);
    if (first == ":" && (rest.empty() || is_full_of_space(rest, 0) == true))
    {
        it->second.setTopic(NO_TOPIC);
        return (true);
    }
    it->second.setTopic(first + rest);
    send_cmd_broadcast(serverDetails, "TOPIC", first + rest, client_fd, it->first);
    // send_msg_to_channel(serverDetails, "TOPIC", first + rest, client_fd, it->first);
    return (true);
}
