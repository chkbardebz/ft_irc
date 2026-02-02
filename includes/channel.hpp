#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include <map>

class Client;

class Channel
{
private :
    std::string _name;
    std::string _topic;
    // std::string _chan_password;
    // int _max_user;
    // bool _is_invite_only;
    // bool _is_topic_restricted;
    // bool _is_chan_password;
    // bool _is_user_limit;
    std::set<int> fds_op;
    std::set<int> fds_channel;

public :
    // Channel();
    Channel(std::string name);
    Channel(const Channel &copy);
    Channel &operator=(const Channel &src);
    ~Channel();

    std::string getName();
    std::set<int> getFds();
    size_t getSize();


    void setTopic(std::string str);

    std::string getTopic();

    void send_msg(std::string message, std::map<int, Client> &huntrill, int client_fd);
    void set_new_fd(int client_fd);
    bool is_fd_in_channel(int client_fd);
    bool client_quit_channel(int client_fd); // PART

    bool is_there_clients(); // PART

};

#endif