#include "../includes/server.hpp"


void print_ip(const struct addrinfo *ai)
{
    char ipstr[INET6_ADDRSTRLEN];

    if (ai->ai_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
        inet_ntop(AF_INET, &ipv4->sin_addr, ipstr, sizeof(ipstr));
        printf("IPv4: %s\n", ipstr);
    }
    else if (ai->ai_family == AF_INET6)
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ai->ai_addr;
        inet_ntop(AF_INET6, &ipv6->sin6_addr, ipstr, sizeof(ipstr));
        printf("IPv6: %s\n", ipstr);
    }
}

int main(int ac, char **av)
{
    struct addrinfo hints, *res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // IMPORTANT

    getaddrinfo(NULL, "6667", &hints, &res);

    int sockfd = socket(res->ai_family,
                        res->ai_socktype,
                        res->ai_protocol);

    bind(sockfd, res->ai_addr, res->ai_addrlen);

    print_ip(res);


    while (1)
    {
        listen(sockfd, 10);
    }

    freeaddrinfo(res);


}


// int main(int ac, char **av)
// {
//     int opt = 1;
//     struct protoent *structproto = getprotobyname("tcp");
//     int sockfd = socket(AF_UNSPEC, SOCK_STREAM, structproto->p_proto);
//     getsockopt(sockfd, structproto->p_proto, SO_REUSEADDR, &opt, (unsigned int *)sizeof(opt));

//     struct sockaddr *addr;



//     bind(sockfd, addr, sizeof(addr));


//     struct sockaddr_in local;
//     socklen_t len = sizeof(local);


//     getsockname(sockfd, (struct sockaddr *)&local, &len);

//     local.sin_port = 6667; //! mettre av2

//     std::cout << inet_ntoa(local.sin_addr) << local.sin_port << std::endl;


// //     while (1)
// //     {

// //     }
// }
