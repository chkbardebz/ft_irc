#include "../includes/ircserv.hpp"
#include "../includes/client.hpp"

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








bool nick_check_duplicate(std::map<int, Client>& huntrill, char *buf)
{
    for (std::map<int, Client>::iterator it = huntrill.begin() ; it != huntrill.end() ; it++)
    {
        std::cout << it->second.getUser() << std::endl;
        if ((std::strcmp(it->second.getUser().c_str(), buf)) == 0)
            return(false);
    }
    return(true);
}

bool nick_set(std::map<int, Client>& huntrill, int newfd)
{
    char buf_nick[1024];
    bool status = false;

    while (status == false)
    {
        write(newfd, "What's your nickname\n> ", 20);
        ssize_t n = recv(newfd, buf_nick, sizeof(buf_nick), 0);
        if (n <= 0)
            return (false);
        buf_nick[n] = '\0';
        
        if (nick_check_duplicate(huntrill, buf_nick) == true)
            status = true;
        else
            write(newfd, "Username already taken!\n", 25);
    }
    Client newclient(buf_nick, "TMP", false);
    huntrill.insert(std::make_pair(newfd, newclient));
    return (true);
}

int setSocketServer(Server &serverDetails)
{
    std::memset(&serverDetails.hints, 0, sizeof(serverDetails.hints));
    serverDetails.hints.ai_family = AF_UNSPEC;
    serverDetails.hints.ai_socktype = SOCK_STREAM;
    serverDetails.hints.ai_flags = AI_PASSIVE; // IMPORTANT
    getaddrinfo(NULL, "6667", &serverDetails.hints, &serverDetails.res);
    return (socket(serverDetails.res->ai_family, serverDetails.res->ai_socktype, serverDetails.res->ai_protocol));
}

void setFds(struct pollfd *fds, int servfd)
{
    fds[0].fd = servfd;
    fds[0].events = POLLIN;

    for (int i = 1; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    Server serverDetails;
    int servfd = setSocketServer(serverDetails);
    bind(servfd, serverDetails.res->ai_addr, serverDetails.res->ai_addrlen);
    print_ip(serverDetails.res);
    listen(servfd, 10);

    struct pollfd fds[MAX_CLIENTS + 1];
    setFds(fds, servfd);

    std::map<int, Client> huntrill;


    while(1)
    {
        poll(fds, MAX_CLIENTS + 1, -1);

        if (fds[0].revents & POLLIN) //? ACCEPTE LES CONNEXIONS ENTRANTES //? fds[0] ==> monitor 
        {
            int clientfd = accept(servfd, NULL, NULL);
            for (int i = 1; i <= MAX_CLIENTS; i++)
            {
                if (fds[i].fd == -1)
                {
                    if (nick_set(huntrill, clientfd) == false)
                    {
                        fprintf(0, "Connexion refusée\n");
                        close(clientfd);
                        break;
                        //! prevoir vrai sortie //! signaux si ctrl C dans user choice
                    }
                    
                    fds[i].fd = clientfd;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }
        for (int i = 1; i <= MAX_CLIENTS; i++) //? RECOIT LES MESSAGES ET LES REDISTRIBUE PARMIS TOUS LES CLIENTS
        {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN))
            {
                char buf[1024];
                ssize_t n = recv(fds[i].fd, buf, sizeof(buf), 0);

                if (n <= 0) //? FERME LE FD CORRESPONDANT AU CLIENT QUI SE DECONNECTE
                {
                    close(fds[i].fd); 
                    fds[i].fd = -1; //? Reset le fd au status "inutilise"
                }
                else
                {
                    
                    for (int j = 1; j <= MAX_CLIENTS; j++)
                        if (j != i)
                            send(fds[j].fd, buf, n, 0);
                }
            }
        }
    }
    freeaddrinfo(serverDetails.res);
}

//todo 
// - comparer comportement USER avec autre irc classique ==> RFC 2812 ??? ...
// - ... finir set_user() 
//? - implementer list // Client  
// - code modulaire ==> remettre au propre ==> channel ?? 
// - les COMMANDES 

// - bonus / envoi de fichiers


























//? Before container map
// bool check_duplicate(Client *allclients, char *buf)
// {
//     for (int i = 0; i < MAX_CLIENTS; i++)
//     {
//         if (strcmp(allclients[i].getUser().c_str(), buf) == 0)
//             return (false);
//     }
//     return (true);
// }

// //! & ou pas ??
// bool check_set_valid_user(Client *allclients, pollfd *fds, int newfd, int i_new_client)
// {
//     (void)fds;
//     char buf[1024];
//     bool status = false;

//     while (status == false)
//     {
//         write(newfd, "QUEL EST TON NOM\n> ", 20);
//         ssize_t n = recv(newfd, buf, sizeof(buf), 0);
//         if (n <= 0)
//             return (false);
//         buf[n - 1] = '\0';  // Enlever le \n de netcat
        
//         if (check_duplicate(allclients, buf) == true)  // ← Vérifier que nick est unique
//             status = true;
//         else
//             write(newfd, "Username already taken!\n", 25);
//     }
//     allclients[i_new_client].setUser(std::string(buf));
//     return (true);
// }

// int main(int ac, char **av)
// {
//     (void)ac;
//     (void)av;

//     struct addrinfo hints, *res;

//     std::memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_UNSPEC;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_flags = AI_PASSIVE; // IMPORTANT

//     getaddrinfo(NULL, "6667", &hints, &res);

//     int servfd = socket(res->ai_family,
//                         res->ai_socktype,
//                         res->ai_protocol);

//     bind(servfd, res->ai_addr, res->ai_addrlen);
//     print_ip(res);
//     listen(servfd, 10);

//     struct pollfd fds[MAX_CLIENTS + 1];
//     Client allclients[MAX_CLIENTS];

//     // std::map<int, Client*> Server;



//     fds[0].fd = servfd;
//     fds[0].events = POLLIN;

//     for (int i = 1; i <= MAX_CLIENTS; i++)
//         fds[i].fd = -1;


//     // char buffer[1024];

//     while(1)
//     {
//         poll(fds, MAX_CLIENTS + 1, -1);

//         if (fds[0].revents & POLLIN) //? ACCEPTE LES CONNEXIONS ENTRANTES //? fds[0] ==> monitor 
//         {
//             int clientfd = accept(servfd, NULL, NULL);
//             for (int i = 1; i <= MAX_CLIENTS; i++)
//             {
//                 if (fds[i].fd == -1)
//                 {
//                     if (check_set_valid_user(allclients, fds, clientfd, i - 1) == false)
//                     {
//                         printf("Connexion refusée\n");
//                         close(clientfd);
//                         break;
//                         //! prevoir vrai sortie //! signaux si ctrl C dans user choice
//                     }
                    
//                     fds[i].fd = clientfd;
//                     fds[i].events = POLLIN;
//                     printf("username : %s\n", allclients[i - 1].getUser().c_str());
//                     break;
//                 }
//             }
//         }
//         for (int i = 1; i <= MAX_CLIENTS; i++) //? RECOIT LES MESSAGES ET LES REDISTRIBUE PARMIS TOUS LES CLIENTS
//         {
//             if (fds[i].fd != -1 && (fds[i].revents & POLLIN))
//             {
//                 char buf[1024];
//                 ssize_t n = recv(fds[i].fd, buf, sizeof(buf), 0);

//                 if (n <= 0) //? FERME LE FD CORRESPONDANT AU CLIENT QUI SE DECONNECTE
//                 {
//                     close(fds[i].fd); 
//                     fds[i].fd = -1; //? Reset le fd au status "inutilise"
//                 }
//                 else
//                 {
                    
//                     for (int i = 1; i <= MAX_CLIENTS; i++)
//                         send(fds[i].fd, buf, n, 0);
//                 }
//             }
//         }
//     }
//     freeaddrinfo(res);
// }








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
