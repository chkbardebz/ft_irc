#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include <map>

# define NO_TOPIC ":No topic is set"

class Client;

class Channel
{
private :
    std::string _name;
    std::string _topic;
    std::string _chan_password;
    int _max_user;
    bool _is_invite_only;
    bool _is_topic_restricted;
    bool _is_chan_password;
    bool _is_user_limit;
    std::set<int> fds_invited;
    std::set<int> fds_op;
    std::set<int> fds_channel;

public :
    // Channel();
    Channel(std::string name);
    Channel(const Channel &copy);
    Channel &operator=(const Channel &src);
    ~Channel();

    const std::string &getName();
    const std::set<int> &getFds();
    size_t getSize();


    void setTopic(std::string str);
    void setMode(char mode, bool add, const char *args);

    void removeOp(int client_fd);
    void setOp(int client_fd);

    const std::string &getTopic();
    bool getTopicStatus(); //! Surement a rename
    bool getUserLimitStatus(); //! pareil
    int getUserLimit();
    bool getPasswordStatus();
    const std::string &getChanPassword();
    void setChanPassword(const std::string &str);
    bool getInviteOnlyStatus();

    void set_new_fd(int client_fd);

    bool is_fd_in_channel(int client_fd);
    
    bool client_quit_channel(int client_fd); // PART

    bool is_there_clients(); // PART

    bool is_fd_op(int client_fd);
    bool is_fd_invited(int client_fd);
    void invite_fd(int fd_invited);
    void remove_invited(int client_fd);

    void send_msg_to_channel(std::string message, std::map<int, Client> &huntrill, int client_fd);

};

#endif