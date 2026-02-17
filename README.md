# ircserv

Simple overview of use/purpose.

## Description

The project consists in reproducing an Internet Relay Chat server. We took RFC-2812 as our reference.

## Getting Started

### Installing

```
git clone git@github.com:chkbardebz/ft_irc.git
cd ft_irc
```

### Executing program

* How to launch the server :
```
make
./ircserv <port> <password>
```
* How to join the server as a client :
```
nc -C <ip_adress> <port>
NICK <nickname>
USER <username> <mode> <unused> <realname>
PASS <password>
```

## Commands

* Common Commands
```
JOIN <channel_name> [<key>] // Join a channel or creates it if it doesn't already exists
PRIVMSG <username/channel_name> :<message>
TOPIC <channel_name> :<new_topic> // if topic restricitions are enabled, only operators can change the topic
INVITE <nickname> <channel> // Sends an invite message to the client (nickname). Mandatory if a client wants to join an invite-only channel
PART <channel_name> [<part_message>] // Removes the client from the channel and sends a message (optional) to all the clients that are in that channel, if the client that quits the channel is the last client in it, the channel is destroyed
QUIT [<quit_message>] // Disconnects the client from the server and sends a message (optional) to all the clients that have at least one channel in common

```
* Operator Commands
```
KICK <channel> <user> [<message>] // Kick an user from a channel sending him optionnaly the reason why he got kicked
MODE <channel> +/-<modes> [<parameters>]
    - i             : turns the invite-only mode on/off
    - k <key>       : sets/unsets a key to the channel
    - o <nickame>   : gives/removes to an user the operator status
    - l <limit>     : sets/unsets an user-limit to the channel
    - t             : sets/unsets the topic restriction to the channel
```

## Authors

* [@aPoet](https://github.com/febouana)
* [@judenis](https://github.com/chkbardebz)

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

Welcome to our ft_irc (Internet Relay Chat)

It's a server program that creates chatrooms like a forum with topics you can specifie

We're doing this project together with aPoet and myself

The program works with two arguments : 
    - port : The port number on which the IRC server will be listening for incoming IRC connections
    - password : The connection password. It will be needed by any IRC client that tries to connect to the server

When the program is running, you can use these 4 Commands :
    - KICK
    - INVITE
    - TOPIC
    - MODE
        . i
        . t
        . k
        . o
        . l

Hope you enjoy !