#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <string>


class Client
{
    private :
        std::string _nickname;
        std::string _username;
        bool _isoperator;
        // std::string _éphropathieshéréditaires;
    public :
        Client(); //? DONE
        ~Client(); //? DONE
        Client(std::string user, std::string nick, bool op); //? DONE
        Client(const Client &src); //? DONE
        Client &operator=(const Client &copy); //? DONE


        int kick(std::string username);
        int invite(std::string username);
        void topic(std::string content);
        int mode(char type);

        std::string getUser(); //? DONE
        std::string getNick(); //? DONE

        void setUser(std::string user); //? DONE
        void setNick(std::string nick); //? DONE
        void setOp(); //? DONE


};

#endif