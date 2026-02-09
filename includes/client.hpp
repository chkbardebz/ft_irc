#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "server.hpp"

# define NOT_INITIALIZED "\0"

class Client
{
    private :
        std::string _nickname;
        std::string _username;
        std::string _realname;
        bool _status_nick;
        bool _status_user;
        bool _status_pass;
        // std::string _éphropathieshéréditaires; 
    public :
        std::string recvBuffer; 
    // == CANON ================================================================================================
        Client();
        Client(const Client &src);
        ~Client(); 
        Client &operator=(const Client &copy);
    // == SETTERS ================================================================================================
        void setUser(std::string username); 
        void setNick(std::string nickname);
        void setReal(std::string realname);

        void setStatusNick(bool new_status); //! I majuscule
        void setStatusUser(bool new_status);
        void setStatusPass(bool new_status);

    // == GETTERS ================================================================================================
        std::string getUser();
        std::string getNick(); 
        std::string getReal();

        bool getStatusNick();
        bool getStatusUser(); 
        bool getStatusPass();

    // == CLEANUP ================================================================================================
        void resetClient();

};

#endif