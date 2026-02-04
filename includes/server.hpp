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
    // == GETTERS ================================================================================================
    std::string getPass();
    struct addrinfo &getHints(); 
    struct addrinfo *getRes() const;
    struct addrinfo **getResAddr();

    std::map<std::string, Channel> makala;
};

// == cmd ==================================================================================================================================================
bool pass(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool nick(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool user(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool privmsg(std::map<int, Client> &huntrill, int client_fd, char *line, Server &serverDetails);
bool join(std::map<int ,Client> &huntrill, int client_fd, char *line, Server &serverDetails);
bool topic(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool part(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool mode(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool invite(std::map<int, Client> &huntrill, int client_fd, char *line, Server &serverDetails);
bool kick(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);

// == irc_utils.cpp ==================================================================================================================================================
bool is_valid_port(char* str);
bool is_valid_char( const std::string str, int i );
bool is_full_of_space(std::string str, size_t i);
void ft_putstr_fd(const char *str, int fd);
bool is_client_set(std::map<int, Client> &huntrill, int client_fd);
void welcome_client(std::map<int, Client> &huntrill, int client_fd);
std::vector<std::string> ft_sukuna(std::string channel_s, char delim);
void clear_vector_sukuned(std::vector<std::string> &channels_splited);

void send_err_msg(std::map<int, Client> &huntrill, int client_fd, int type_err, std::string reason, std::string message);
// void send_msg_to_client_with_fd(std::map<int, Client> &huntrill, std::string cmd, std::string message, int receiver_fd, int sender_fd);

bool send_msg_to_channel(Server &serverClient, std::map<int, Client> &huntrill, std::string cmd, std::string message, int sender_fd, std::string channel);
bool send_msg_to_client(std::map<int, Client> &huntrill, int sender_fd, int receiver_fd, std::string cmd, std::string message);
bool send_cmd_broadcast(Server &serverDetails, std::map<int, Client> &huntrill, std::string cmd, std::string message, int sender_fd, std::string channel);
int nick_to_fd(std::map<int, Client> &huntrill, std::string nick);



bool is_already_registered(std::map<int, Client> &huntrill, int client_fd);

#endif
