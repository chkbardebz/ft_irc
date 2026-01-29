#include "../includes/server.hpp"

bool join(std::map<int ,Client> &huntrill, int client_fd, char *line, Server &serverDetails)
{
    (void)huntrill;
    std::stringstream ss(line);
    std::string cmd, name; //, passwd;

    ss >> cmd >> name;
    std::map<std::string, Channel>::iterator it = serverDetails.makala.begin();
    if (name[0] != '#')
        return (write(client_fd, "999 ERR_BADCHANMASK\n", 21), false);
    if (name.empty())
        return (write(client_fd, "999 ERR_NEEDMOREPARAMS\n", 24), false);
    // getline(ss, passwd); //! rajouter lorsque MODE pret
    for (; it != serverDetails.makala.end(); it++)
    {
        if (strcmp(name.c_str(), it->second.getName().c_str()) == 0)
        {
            it->second.set_new_fd(client_fd);
            return (true);
        }
    }
    Channel new_class(name);
    serverDetails.makala.insert(std::make_pair(name, new_class));
    it = serverDetails.makala.find(name);
    it->second.set_new_fd(client_fd);
    return (true);
}