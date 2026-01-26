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


int setSocketServer(Server &serverDetails)
{
    std::memset(&serverDetails.hints, 0, sizeof(serverDetails.hints));
    serverDetails.hints.ai_family = AF_UNSPEC;
    serverDetails.hints.ai_socktype = SOCK_STREAM;
    serverDetails.hints.ai_flags = AI_PASSIVE; // IMPORTANT
    getaddrinfo(NULL, "6667", &serverDetails.hints, &serverDetails.res);
    int servsocket = socket(serverDetails.res->ai_family, serverDetails.res->ai_socktype, serverDetails.res->ai_protocol);
    int opt = 1;
    if (setsockopt(servsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) //? pour eviter le cooldown de con
    {
        perror("setsockopt SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }
    return (servsocket);
}

void setFds(struct pollfd *fds, int servfd)
{
    fds[0].fd = servfd;
    fds[0].events = POLLIN;
    for (int i = 1; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;
}

bool isValidChar( const std::string str, int i )
{
    for (; str[i] ; i++)
    {
        if (std::isalpha(str[i]) == false && std::isdigit(str[i]) == false)
            return (false);
    }
    return (true);
}

bool nick_check_duplicate(std::map<int, Client>& huntrill, std::string nickname)
{
    for (std::map<int, Client>::iterator it = huntrill.begin() ; it != huntrill.end() ; it++)
    {
        if ((std::strcmp(it->second.getNick().c_str(), nickname.c_str())) == 0)
            return(false);
    }
    return(true);
}

// std::string cmd, nickname; tres tres utile pour command avec plusieurs args comme PRIVMSG / CHANNEL... etc
// nick peut se renommer pas user
bool nick_set(std::map<int, Client> &huntrill, int client_fd, char* line)
{
    std::stringstream ss(line);
    std::string cmd, nickname; //, more

    ss >> cmd >> nickname; // >> more
    if (nickname.empty()) //? err: pas de nickname fourni
    {
        write(client_fd, "431 ERR_NONICKNAMEGIVEN\n", 21);
        return (false);
    }
    // if (!more.empty()) //? err: pas de nickname fourni //! si plusieurs args envoye seul le premier est retenu pour devenir le nickname le reste est ignore + pas de msg d'err
    // {
    //     write(client_fd, "t'as cru quoi r n'a change la ptn dta mere.\n", 45);
    //     return (false);
    // }
    if (nick_check_duplicate(huntrill, nickname) == false) // err: nickname deja existant 
    {
        write(client_fd, "433 ERR_NICKNAMEINUSE\n", 23);
        return (false);
    }
    if (std::isdigit(nickname[0]) || !isValidChar(nickname, 1) || nickname.size() > 9) // err: first elem isdigit
    {
        write(client_fd, "432 ERR_ERRONEUSNICKNAME\n", 26);
        return (false);
    }

    std::map<int, Client>::iterator it = huntrill.find(client_fd);
    if (it != huntrill.end())  // Vérifier que le client existe
        it->second.setNick(nickname);
    // std::cout << "STATUS: " << std::endl;
    // std::cout << "client/fd: " << it->first << std::endl;
    // std::cout << "nick: " << it->second.getNick() << std::endl;
    // std::cout << "user: " << it->second.getUser() << std::endl;
    return (true);
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

bool Error(std::map<int, Client> &huntrill, int client_fd, char* line)
{
    (void)huntrill;
    (void)client_fd;
    (void)line;
    std::cout << "vaffanculo\n";
    return (true);
}


bool fullisspace(std::string str, int i)
{
    for(; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (false);
    }
    return (true);
}


bool user_set(std::map<int, Client> &huntrill, int client_fd, char* line)
{
    std::map<int, Client>::iterator it = huntrill.find(client_fd); //! pas de verif si it == huntrill.end() ?
    if (it->second.getUser() != NO_USERNAME)
    {
        write(client_fd, "462 ERR_ALREADYREGISTRED USER\n", 31);
        return (false);
    }
    std::stringstream ss(line);
    std::string cmd, username, hostname, servname, realname;
    if (!(ss >> cmd >> username >> hostname >> servname))
    {
        write(client_fd, "461 ERR_NEEDMOREPARAMS USER\n", 29);
        return (false);
    }
    getline(ss, realname);
    while (realname[0] && isspace(realname[0]))
        realname.erase(0, 1);
    if (realname.empty() || realname[0] != ':' || fullisspace(realname, 2) == true)
    {
        write(client_fd, "461 ERR_NEEDMOREPARAMS USER\n", 29);
        return (false);
    }
    realname.erase(0, 1); //? supp le ':'
    it->second.setUser(username);
    it->second.setReal(realname);
    return (true);
}


void AcceptNewCommand(std::map<int, Client>& huntrill, struct pollfd *fds)
{
    bool (*funcs[])(std::map<int, Client> &huntrill, int client_fd, char* line) = {&nick_set, &user_set, &privmsg, &Error};
    std::string cmd_names[] = {"NICK", "USER", "PRIVMSG","ERROR"};

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
            for (size_t j = 0; j < 3 ; j++)
            {
                if (std::strcmp(cmd.c_str(), cmd_names[j].c_str()) == 0)
                    funcs[j](huntrill, fds[i].fd, line_buf); //je n'appel pas un ptr sur "fn libre" mais sur des methodes membres de la classe intern d'ou l'utilisation de this->
            }
        }
    }
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

        acceptNewConnexion(huntrill, fds, servfd);
        AcceptNewCommand(huntrill, fds);
    }
    freeaddrinfo(serverDetails.res);
}

// todo 
// - des qu'on arrive brianstorming : toi tu me rara tout et moi j'explique mon bebe + ca me fait revise


//todo 
//? - comparer comportement USER avec autre irc classique ==> RFC 2812 ??? ...
//? - ... finir nick_set() 
//? - implementer list // Client  
//? - code modulaire ==> remettre au propre ==> channel ?? 
//? - prevoir si un client veut se connecter mais plus de place
// - les COMMANDES 



// - fonction quit + signaux + mettre des try si necessaire pour eviter des possibles bad_alloc et autre

// - bonus / envoi de fichiers























// bool user_set(std::map<int, Client> &huntrill, int client_fd, char* line)
// {
//     write(client_fd, "queeeeeeeeeeeeeeeeeetteeeeeeeeeee\n", 35); //!

//     std::map<int, Client>::iterator it = huntrill.find(client_fd);
//     // if (it == huntrill.end()) //! theoriquement Client ne peux pas rentrer en etant pas initialiser #impossibletkt
//     // {
//     //     write(client_fd, "jtema ton posterieur jveux le meme en po-po-ster (hmmm ouais)\n", 62);
//     //     return (false); 
//     // }
//     // if (std::strcmp(it->second.getUser().c_str(), NO_USERNAME)) //? Vérifier que le client n'a pa deja rentre son username
//     if (it->second.getUser() != NO_USERNAME)
//     {
//         write(client_fd, "462 ERR_ALREADYREGISTRED USER\n", 31);
//         return (false);
//     }

//     std::stringstream ss(line);
//     std::string cmd, username, hostname, servname, realname;
//     if (!(ss >> cmd >> username >> hostname >> servname)) //? Si un parametre est manquant 
//     {
//         write(client_fd, "461 ERR_NEEDMOREPARAMS USER\n", 29);
//         return (false);
//     }
//     // if (std::strcmp(hostname.c_str(), "0") || std::strcmp(servname.c_str(), "*")) //? NE RENVOI PAS DE MSG D'ERREURS POUR CE CAS
//     // {
//     //     write(client_fd, "gougougaga mauvai\n", 21);
//     //     return (false);
//     // }
//     getline(ss, realname);
//     if (realname.empty() || realname[1] != ':') //? Si realname est manquant OU si ':' est absent 
//     {
//         write(client_fd, "461 ERR_NEEDMOREPARAMS USER\n", 29);
//         return (false);
//     }
//     realname.erase(0, 2); //? supp l'espace initial
//     // std::cout << "REALNAME=" << realname << std::endl;
//     // write(0, realname.c_str(), strlen(realname.c_str()));



//     it = huntrill.find(client_fd); //! Normalement ne tombe jamais dans ce cas erreur, comment verifier si client existe ?? 
//     it->second.setUser(username);
//     it->second.setReal(realname);

//     // std::cout << "STATUS: " << std::endl;
//     // std::cout << "client/fd: " << it->first << std::endl;
//     // std::cout << "nick: " << it->second.getNick() << std::endl;
//     // std::cout << "user: " << it->second.getUser() << std::endl;
//     return (true);
// }

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

//     local.sin_port = 6667; 

//     std::cout << inet_ntoa(local.sin_addr) << local.sin_port << std::endl;


// //     while (1)
// //     {

// //     }
// }
