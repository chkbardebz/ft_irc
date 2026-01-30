#include "../includes/server.hpp"

bool join(std::map<int ,Client> &huntrill, int client_fd, char *line, Server &serverDetails)
{
    (void)huntrill;
    std::stringstream ss(line);
    std::string cmd, name, passwd;

    if (!(ss >> cmd >> name))
        return (write(client_fd, "999 ERR_NEEDMOREPARAMS\n", 24), false);
    std::map<std::string, Channel>::iterator it = serverDetails.makala.begin();
    if (name[0] != '#')
        return (write(client_fd, "999 ERR_BADCHANMASK\n", 21), false);
    getline(ss, passwd);
    for (; it != serverDetails.makala.end(); it++)
    {
        if (strcmp(name.c_str(), it->second.getName().c_str()) == 0)
        {
            if (it->second.getUserLimitStatus() == true && it->second.getFds().size() + 1 > (size_t)it->second.getUserLimit())
                return (write(client_fd, "471 ERR_CHANNELISFULL\n", 23), false);
            if (it->second.getPasswordStatus() == true)
            {
                std::stringstream ss_passwd(passwd);
                if (!(ss_passwd>>passwd))
                    return (write(client_fd, "975 ERR_BADCHANNELKEY\n", 23), false);
                // std::cout << "JOIN PASSWORD = |" << passwd << "|" << std::endl;
                // std::cout << "CHANNEL PASSWORD = |" << it->second.getChanPassword() << "|" << std::endl;
                if (passwd != it->second.getChanPassword())
                    return (write(client_fd, "975 ERR_BADCHANNELKEY\n", 23), false);
            }
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