#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <string>

# define NO_NICKNAME "no_nickname"
# define NO_USERNAME "no_username"
# define NO_REALNAME "no_realname"

class Client
{
    private :
        std::string _nickname;
        std::string _username;
        std::string _realname;
        // std::string _éphropathieshéréditaires; 
    public :
    // == CANON ================================================================================================
        Client();
        ~Client(); 
        Client(std::string nick, std::string user, std::string realname);
        Client(const Client &src);
        Client &operator=(const Client &copy);
    // == SETTERS ================================================================================================
        void setUser(std::string username); 
        void setNick(std::string nickname);
        void setReal(std::string realname);

    // == GETTERS ================================================================================================
        std::string getUser();
        std::string getNick(); 
        std::string getReal();


};

#endif