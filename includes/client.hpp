#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <string>

class Client
{
    private :
        bool isoperator;
        std::string _username;
        std::string _nickname;
        std::string _éphropathieshéréditaires;
    public :
        Client();
        ~Client();
        Client(std::string user, std::string nick, bool op);
        Client(const Client &src);
        Client &operator=(const Client &copy);


        int kick(std::string username);
        int invite(std::string username);
        void topic(std::string content);
        int mode(char type);

        std::string getUser();
        std::string getNick();

        int setOp();


};

#endif