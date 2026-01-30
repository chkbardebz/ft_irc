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

#define NO_PASS "\0"
#define MAX_CLIENTS 42

// struct Server 
// {
//     std::string _password;
//     struct addrinfo hints;
//     struct addrinfo *res;
// };

class Server
{
private:
    std::string _password;
    struct addrinfo _hints;
    struct addrinfo *_res;
    // Server(const Server& copy); //interdire les copy profonde + corriger ptet //!
    // Server &operator=(const Server& copy); //interdire les copy profonde + corriger ptet //!
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

bool pass(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool nick(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool user(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool privmsg(std::map<int, Client> &huntrill, int client_fd, char *line, Server &serverDetails);
bool join(std::map<int ,Client> &huntrill, int client_fd, char *line, Server &serverDetails);
bool topic(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);
bool part(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails);


void is_client_welcome(std::map<int, Client> &huntrill, int client_fd);
bool is_client_set(std::map<int, Client> &huntrill, int client_fd);

bool check_if_fullofsapce(std::string str); //! mettre dans utils



bool send_msg_to_channel(Server &serverDetails, std::map<int, Client> &huntrill, int client_fd, std::string channel, std::string message);
#endif

    