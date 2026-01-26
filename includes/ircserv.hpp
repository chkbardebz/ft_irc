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

# include <stdio.h> //!
# include <stdlib.h> //!

# include <map>
# include "client.hpp"

#include <sstream> //parsing

#define MAX_CLIENTS 42

struct Server 
{
    struct addrinfo hints;
    struct addrinfo *res;
};

bool privmsg(std::map<int, Client> &huntrill, int client_fd, char *line);

#endif