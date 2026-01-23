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

#define MAX_CLIENTS 10

struct Server 
{
    struct addrinfo hints;
    struct addrinfo *res;
};

#endif