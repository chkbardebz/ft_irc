#include "../includes/server.hpp"

void print_ip(const struct addrinfo *ai)
{
    char ipstr[INET6_ADDRSTRLEN];

    if (ai->ai_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
        inet_ntop(AF_INET, &ipv4->sin_addr, ipstr, sizeof(ipstr));
        std::cout << "IPv4: " << ipstr << std::endl;
    }
    else if (ai->ai_family == AF_INET6)
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ai->ai_addr;
        inet_ntop(AF_INET6, &ipv6->sin6_addr, ipstr, sizeof(ipstr));
        std::cout << "IPv6: " << ipstr << std::endl;
    }
}

void setFds(struct pollfd *fds, int servfd)
{
    fds[0].fd = servfd;
    fds[0].events = POLLIN;
    for (int i = 1; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;
}

void acceptNewConnexion(std::map<int, Client>& huntrill, struct pollfd *fds, int servfd)
{
    int i = 1;
    if (fds[0].revents & POLLIN) //? ACCEPTE LES CONNEXIONS ENTRANTES //? fds[0] ==> monitor 
    {
        int new_client_fd = accept(servfd, NULL, NULL);
        while (i <= MAX_CLIENTS)
        {
            if (fds[i].fd == -1)
            {
                fds[i].fd = new_client_fd;
                fds[i].events = POLLIN;
                Client new_client_class; //? default constructor
                huntrill.insert(std::make_pair(new_client_fd, new_client_class));
                break;
            }
            i++;
        }
        if (i == MAX_CLIENTS)
        {
            write(new_client_fd, "Err: no no noooooo\n", 20);
            close(new_client_fd); 
        }
    }
}


void AcceptNewCommand(std::map<int, Client>& huntrill, struct pollfd *fds, Server &serverDetails)
{
    bool (*funcs[])(std::map<int, Client> &huntrill, int client_fd, char* line, Server &serverDetails) = {&nick, &user, &privmsg, &pass, &join, &topic, &mode};
    std::string cmd_names[] = {"NICK", "USER", "PRIVMSG", "PASS", "JOIN", "TOPIC", "MODE"};

    for (int i = 1; i <= MAX_CLIENTS; i++) //? RECOIT LES MESSAGES ET LES REDISTRIBUE PARMIS TOUS LES CLIENTS
    {
        if (fds[i].fd != -1 && (fds[i].revents & POLLIN))
        {
            char line_buf[1024];
            ssize_t n = recv(fds[i].fd, line_buf, sizeof(line_buf), 0);
            line_buf[n] = '\0';
            std::stringstream ss(line_buf);
            std::string cmd;
            ss >> cmd;
            if (n <= 0) //? FERME LE FD CORRESPONDANT AU CLIENT QUI SE DECONNECTE 
            { 

                //! a mettre dans la fonction quit propre a faire
                huntrill.erase(fds[i].fd);
                close(fds[i].fd); 
                fds[i].fd = -1; //? Reset le fd au status "inutilise"
            }
            for (size_t j = 0; j < 7 ; j++)
            {
                if (std::strcmp(cmd.c_str(), cmd_names[j].c_str()) == 0)
                    funcs[j](huntrill, fds[i].fd, line_buf, serverDetails); //je n'appel pas un ptr sur "fn libre" mais sur des methodes membres de la classe intern d'ou l'utilisation de this->
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

int main(int ac, char **av)
{
    if (ac != 3)
        return(write(2, "Usage: ./ircserv <port> <password>\n", 36), 1);
    Server serverDetails;
    serverDetails.setPass(std::string(av[2]));
    int servfd = setSocketServer(serverDetails, av[1]);
    if (servfd < 0)
        return (write(2, "Error: socket creation failed\n", 30), 1);
    if (bind(servfd, serverDetails.getRes()->ai_addr, serverDetails.getRes()->ai_addrlen) < 0)
        return (write(2, "Error: bind failed\n", 19), 1);
    print_ip(serverDetails.getRes());
    if (listen(servfd, 10) < 0)
        return (write(2, "Error: listen failed\n", 21), 1);
    struct pollfd fds[MAX_CLIENTS + 1];
    setFds(fds, servfd);
    std::map<int, Client> huntrill;
    while(1)
    {
        if (poll(fds, MAX_CLIENTS + 1, -1) < 0)
        {
            write(2, "Error: Poll failed to initialize\n", 34);
            break;
        }
        acceptNewConnexion(huntrill, fds, servfd);
        AcceptNewCommand(huntrill, fds, serverDetails);
    }
    freeaddrinfo(serverDetails.getRes());
}


//todo 
//* - les COMMANDES : CHANNEL
//  - penser a reset Client lorsque le fd se deco -> already registered

// - changer parsing rajouter verif isdigit pour port par exemple et autre 
// - couper fn privmsg en sous fonction pour gerer les #channels


// - fonction quit + signaux + mettre des try si necessaire pour eviter des possibles bad_alloc et autre
// - bonus / envoi de fichiers
