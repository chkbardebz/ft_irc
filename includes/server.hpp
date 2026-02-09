#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <cstring>
# include <unistd.h>
# include <poll.h>
# include <stdlib.h>
# include <signal.h>

# include <map>
# include "client.hpp"
# include "channel.hpp"


# include <sstream> //parsing
# include <set>
# include <vector> //! remplacer dans part par set ?

#define NO_PASS "\0"
#define MAX_CLIENTS 42

class Server
{
private:
    std::string _password;
    struct addrinfo _hints;
    struct addrinfo *_res;
    struct pollfd *_fds;
    // Server(const Server& copy); //? interdire les copy profonde + corriger ptet
    // Server &operator=(const Server& copy); //? interdire les copy profonde + corriger ptet
public:
    // == CANON ================================================================================================
    Server();
    Server(std::string password);
    ~Server();
    // == SETTERS ================================================================================================
    void setPass(std::string username); 
    void setHints(const struct addrinfo& _hints);
    void setRes(struct addrinfo *_res);
    void setFds(struct pollfd *fds);
    // == GETTERS ================================================================================================
    std::string getPass();
    struct addrinfo &getHints(); 
    struct addrinfo *getRes() const;
    struct addrinfo **getResAddr();
    struct pollfd *getFds();

    std::map<int, Client> huntrill;
    std::map<std::string, Channel> makala;
};

// == cmd ==================================================================================================================================================
bool pass(int client_fd, std::string line, Server &serverDetails);
bool nick(int client_fd, std::string line, Server &serverDetails);
bool user(int client_fd, std::string line, Server &serverDetails);
bool privmsg(int client_fd, std::string line, Server &serverDetails);
bool join(int client_fd, std::string line, Server &serverDetails);
bool topic(int client_fd, std::string line, Server &serverDetails);
bool part(int client_fd, std::string line, Server &serverDetails);
bool mode(int client_fd, std::string line, Server &serverDetails);
bool invite(int client_fd, std::string line, Server &serverDetails);
bool kick(int client_fd, std::string line, Server &serverDetails);
bool quit(int client_fd, std::string line, Server &serverDetails);

// == irc_utils.cpp ==================================================================================================================================================
bool is_valid_port(char* str);
bool is_valid_char( const std::string str, int i );
bool is_full_of_space(std::string str, size_t i);
void welcome_client(Server &serverDetails, int client_fd);
std::vector<std::string> ft_sukuna(std::string channel_s, char delim); //! vaffanculo
void clear_vector_sukuned(std::vector<std::string> &channels_splited); //! vaffanculo

void send_err_msg(Server &serverDetails, int client_fd, int type_err, std::string reason, std::string message);
// void send_msg_to_client_with_fd(std::map<int, Client> &huntrill, std::string cmd, std::string message, int receiver_fd, int sender_fd);

bool send_msg_to_channel(Server &serverClient, std::string cmd, std::string message, int sender_fd, std::string channel);
bool send_msg_to_client(Server &serverDetails, int sender_fd, int receiver_fd, std::string cmd, std::string message);
bool send_cmd_broadcast(Server &serverDetails, std::string cmd, std::string message, int sender_fd, std::string channel);
int nick_to_fd(Server &serverDetails, std::string nick);

void handle_sigint(int sig);
bool is_already_registered(Server &serverDetails, int client_fd);

#endif
