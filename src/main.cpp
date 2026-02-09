#include "../includes/server.hpp"

// void print_ip(const struct addrinfo *ai)
// {
//     char ipstr[INET6_ADDRSTRLEN];

//     if (ai->ai_family == AF_INET)
//     {
//         struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
//         inet_ntop(AF_INET, &ipv4->sin_addr, ipstr, sizeof(ipstr));
//         std::cout << "IPv4: " << ipstr << std::endl;
//     }
//     else if (ai->ai_family == AF_INET6)
//     {
//         struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ai->ai_addr;
//         inet_ntop(AF_INET6, &ipv6->sin6_addr, ipstr, sizeof(ipstr));
//         std::cout << "IPv6: " << ipstr << std::endl;
//     }
// }

void setFds(struct pollfd *fds, int servfd)
{
    fds[0].fd = servfd;
    fds[0].events = POLLIN;
    for (int i = 1; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;
}

                #include <cerrno>   // pour errno
                #include <cstring>  // pour strerror
                #include <cstdio>   // pour printf
void acceptNewConnexion(Server &serverDetails, struct pollfd *fds, int servfd)
{
    int i = 1;
    if (fds[0].revents & POLLIN) //? ACCEPTE LES CONNEXIONS ENTRANTES //? fds[0] ==> monitor 
    {
        int new_client_fd = accept(servfd, NULL, NULL); //! == open()
        while (i <= MAX_CLIENTS)
        {
            if (fds[i].fd == -1)
            {
                fds[i].fd = new_client_fd; //! Deux trucs a close() ???
                fds[i].events = POLLIN;
                Client new_client_class; //? default constructor
                serverDetails.huntrill.insert(std::make_pair(new_client_fd, new_client_class));
                break;
            }
            i++;
        }
        if (i == MAX_CLIENTS)
        {
            std::cout << "Error: max clients allowed is " << MAX_CLIENTS << std::endl;
            close(new_client_fd); 
        }
    }
}

void AcceptNewCommand(Server &serverDetails, struct pollfd *fds)
{
    bool (*funcs[])(int client_fd, std::string line, Server &serverDetails) = {&nick, &user, &privmsg, &pass, &join, &topic, &part, &invite, &mode, &kick, &quit};
    std::string cmd_names[] = {"NICK", "USER", "PRIVMSG", "PASS", "JOIN", "TOPIC", "PART", "INVITE", "MODE", "KICK", "QUIT"};

    for (int i = 1; i <= MAX_CLIENTS; i++) //? RECOIT LES MESSAGES ET LES REDISTRIBUE PARMIS TOUS LES CLIENTS
    {
        if (fds[i].fd != -1 && (fds[i].revents & POLLIN))
        {
            char line_buf[512];
            ssize_t n = recv(fds[i].fd, line_buf, sizeof(line_buf) - 1, 0);
            std::map<int, Client>::iterator it = serverDetails.huntrill.find(fds[i].fd);
            if (n > 0)
                it->second.recvBuffer.append(line_buf, n);
            if (n <= 0 || n >= 512 || it->second.recvBuffer.size() >= 512) //? FERME LE FD CORRESPONDANT AU CLIENT QUI SE DECONNECTE 
            {
                if (n >= 512)
                    send(fds[i].fd, "QUIT: Suspicious behavior\r\n", 28, 0);
                funcs[10](fds[i].fd, std::string("QUIT :Connection Interupted\r\n"), serverDetails); //? Deconnection impromptue
                close(fds[i].fd); 
                fds[i].fd = -1; //? Reset le fd au status "inutilise"
                it->second.recvBuffer.clear();
                continue ;
            }
            // line_buf[n] = '\0';

            if (it->second.recvBuffer.size() >= 2 && it->second.recvBuffer[it->second.recvBuffer.size() - 2] == '\r' && it->second.recvBuffer[it->second.recvBuffer.size() - 1] == '\n')
            {
                std::stringstream ss(it->second.recvBuffer);
                std::string cmd;
                ss >> cmd;
                bool valid_cmd = false;
                for (size_t j = 0; j < 11 ; j++)
                {
                    if (std::strcmp(cmd.c_str(), cmd_names[j].c_str()) == 0)
                    {
                        valid_cmd = true;
                        funcs[j](fds[i].fd, it->second.recvBuffer, serverDetails);

                        if (cmd == "QUIT")
                        {
                            close(fds[i].fd);
                            fds[i].fd = -1;
                        }
                        break;
                    }
                }
                if (!valid_cmd && !cmd.empty())
                    send_err_msg(serverDetails, fds[i].fd, 421, cmd,":Unknown command"); //ERR_UNKNOWNCOMMAND
                it->second.recvBuffer.clear();
            }
        }
    }
}

int setSocketServer(Server &serverDetails, char *port)
{
    std::memset(&serverDetails.getHints(), 0, sizeof(struct addrinfo));
    serverDetails.getHints().ai_family = AF_UNSPEC;
    serverDetails.getHints().ai_socktype = SOCK_STREAM;
    serverDetails.getHints().ai_flags = AI_PASSIVE; // IMPORTANT
    getaddrinfo(NULL, port, &serverDetails.getHints(), serverDetails.getResAddr());
    int servsocket = socket(serverDetails.getRes()->ai_family, serverDetails.getRes()->ai_socktype, serverDetails.getRes()->ai_protocol);
    int opt = 1;
    if (setsockopt(servsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) //? pour eviter le cooldown de con
    {
        write(2, "Error: setsockopt SO_REUSEADDR\n", 32);
        exit(1); //! //quit() ?
    }
    return (servsocket);
}


// std::cout << "STATUS: " << std::endl;
// std::cout << "client/fd: " << it->first << std::endl;
// std::cout << "nick: " << it->second.getNick() << std::endl;
// std::cout << "user: " << it->second.getUser() << std::endl;

Server serverDetails;

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        std::cout << "crtl + c" << std::endl;
        serverDetails.makala.erase(serverDetails.makala.begin(), serverDetails.makala.end());
        for (std::map<int, Client>::iterator it_client = serverDetails.huntrill.begin(); it_client != serverDetails.huntrill.end(); it_client++)
        {
            close(it_client->first);
        }
        serverDetails.huntrill.erase(serverDetails.huntrill.begin(), serverDetails.huntrill.end());
        struct pollfd *tmp = serverDetails.getFds();
        
        for (int i = 2; i != MAX_CLIENTS; i++)
            tmp[i].fd = -1;
        freeaddrinfo(serverDetails.getRes());
        exit (1);
    }
}

int main(int ac, char **av)
{
    if (ac != 3)
        return(write(2, "Usage: ./ircserv <port> <password>\n", 36), 1);
    if (is_valid_port(av[1]) == false)

        return (write(2, "Error: Port must be numeric\n", 29), 1);
    serverDetails.setPass(std::string(av[2]));

    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = handle_sigint;

    int servfd = setSocketServer(serverDetails, av[1]);
    if (servfd < 0)
        return (write(2, "Error: socket creation failed\n", 30), 1);
    if (bind(servfd, serverDetails.getRes()->ai_addr, serverDetails.getRes()->ai_addrlen) < 0)
        return (write(2, "Error: bind failed\n", 19), 1);
    // print_ip(serverDetails.getRes());
    if (listen(servfd, 10) < 0)
        return (write(2, "Error: listen failed\n", 21), 1);
    struct pollfd fds[MAX_CLIENTS + 1];
    setFds(fds, servfd);
    serverDetails.setFds(fds);
    
    while(1)
    {
        if (poll(fds, MAX_CLIENTS + 1, -1) < 0)
        {
            write(2, "Error: Poll failed to initialize\n", 34);
            break;
        }
        acceptNewConnexion(serverDetails, fds, servfd);
        AcceptNewCommand(serverDetails, fds);
    }
    freeaddrinfo(serverDetails.getRes());
}


//todo list
// - //signaux + Corriger toutes les notes (//reset client) + free PARTOUT + shutdown le serv
// - securiser chaque fn avec if si besoin + mettre des try si necessaire pour eviter des possibles bad_alloc et autre