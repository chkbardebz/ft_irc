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
    // int client_fd[10];

    struct addrinfo hints, *res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // IMPORTANT

    getaddrinfo(NULL, "6667", &hints, &res);

    int servfd = socket(res->ai_family,
                        res->ai_socktype,
                        res->ai_protocol);

    bind(servfd, res->ai_addr, res->ai_addrlen);
    print_ip(res);
    listen(servfd, 10);

    struct pollfd fds[MAX_CLIENTS + 1];

    fds[0].fd = servfd;
    fds[0].events = POLLIN;

    for (int i = 1; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;


    // char buffer[1024];

    while(1)
    {
        poll(fds, MAX_CLIENTS + 1, -1);

        if (fds[0].revents & POLLIN)
        {
            int clientfd = accept(servfd, NULL, NULL);
            for (int i = 1; i <= MAX_CLIENTS; i++)
            {
                if (fds[i].fd == -1)
                {
                    fds[i].fd = clientfd;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }
        for (int i = 1; i <= MAX_CLIENTS; i++)
        {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN))
            {
                char buf[1024];
                ssize_t n = recv(fds[i].fd, buf, sizeof(buf), 0);

                if (n <= 0)
                {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                else
                {
                    for (int i = 1; i <= MAX_CLIENTS; i++)
                        send(fds[i].fd, buf, n, 0);
                }
            }
        }
    }
    freeaddrinfo(res);
}


//   for (int i = 0; i<2; i++)
//         {
//             ssize_t bytes = recv(client_fd[i], buffer, sizeof(buffer) - 1, 0);
//             if (bytes <= 0)
//                 break;
//             buffer[bytes] = '\0';
//             printf("Client: %s\n", buffer);
//             send(client_fd[i], buffer, bytes, 0);
//         }

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
