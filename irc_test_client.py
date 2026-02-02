#!/usr/bin/env python3
"""
Script de test automatique pour serveur IRC ft_irc
Automatise l'authentification (PASS, NICK, USER) et permet de tester les commandes
"""

import socket
import sys
import time
import select
import argparse


class IRCTestClient:
    def __init__(self, host='localhost', port=6667, password='fdp', nickname='testuser', 
                 username='testuser', realname='Test User', auto_join=None):
        self.host = host
        self.port = int(port)
        self.password = password
        self.nickname = nickname
        self.username = username
        self.realname = realname
        self.auto_join = auto_join
        self.sock = None
        
    def connect(self):
        """Établit la connexion au serveur"""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print(f"[*] Connexion à {self.host}:{self.port}...")
            self.sock.connect((self.host, self.port))
            print("[✓] Connecté au serveur IRC!")
            return True
        except Exception as e:
            print(f"[!] Erreur de connexion: {e}")
            return False
    
    def send_command(self, command, show_response=True, delay=0.2):
        """Envoie une commande et affiche la réponse"""
        try:
            print(f"[→] {command.strip()}")
            self.sock.send(f"{command}\r\n".encode())
            time.sleep(delay)
            
            if show_response:
                try:
                    self.sock.setblocking(False)
                    response = self.sock.recv(4096).decode('utf-8', errors='ignore')
                    if response:
                        print(f"[←] {response}", end='')
                    self.sock.setblocking(True)
                except BlockingIOError:
                    pass
            return True
        except Exception as e:
            print(f"[!] Erreur lors de l'envoi: {e}")
            return False
    
    def authenticate(self):
        """Effectue l'authentification complète (PASS, NICK, USER)"""
        print("\n" + "="*50)
        print("AUTHENTIFICATION AUTOMATIQUE")
        print("="*50)
        
        # Étape 1: PASS
        if self.password:
            if not self.send_command(f"PASS {self.password}"):
                return False
        else:
            print("[!] Aucun mot de passe fourni")
        
        # Étape 2: NICK
        if not self.send_command(f"NICK {self.nickname}"):
            return False
        
        # Étape 3: USER
        if not self.send_command(f"USER {self.username} 0 * :{self.realname}"):
            return False
        
        print("\n[✓] Authentification terminée!")
        
        # Auto-join si spécifié
        if self.auto_join:
            time.sleep(0.3)
            print("\n" + "="*50)
            print("AUTO-JOIN CHANNELS")
            print("="*50)
            for channel in self.auto_join:
                if not channel.startswith('#'):
                    channel = '#' + channel
                self.send_command(f"JOIN {channel}")
        
        return True
    
    def interactive_mode(self):
        """Mode interactif après authentification"""
        print("\n" + "="*50)
        print("MODE INTERACTIF - Tapez vos commandes IRC")
        print("="*50)
        print("Commandes disponibles:")
        print("  JOIN #channel          - Rejoindre un channel")
        print("  PART #channel :msg     - Quitter un channel")
        print("  PRIVMSG #channel :msg  - Envoyer un message")
        print("  TOPIC #channel :topic  - Changer le topic")
        print("  QUIT :msg              - Déconnexion")
        print("  Ctrl+C                 - Quitter le script")
        print("="*50 + "\n")
        
        self.sock.setblocking(False)
        
        try:
            while True:
                # Lire les réponses du serveur
                try:
                    data = self.sock.recv(4096)
                    if data:
                        print(data.decode('utf-8', errors='ignore'), end='')
                    else:
                        print("\n[!] Connexion fermée par le serveur")
                        break
                except BlockingIOError:
                    pass
                except Exception as e:
                    print(f"\n[!] Erreur de réception: {e}")
                    break
                
                # Lire depuis stdin
                if select.select([sys.stdin], [], [], 0.1)[0]:
                    line = sys.stdin.readline()
                    if line:
                        try:
                            self.sock.send(line.encode())
                        except Exception as e:
                            print(f"[!] Erreur d'envoi: {e}")
                            break
                            
        except KeyboardInterrupt:
            print("\n\n[*] Interruption par l'utilisateur")
        finally:
            self.disconnect()
    
    def disconnect(self):
        """Ferme la connexion proprement"""
        if self.sock:
            print("[*] Déconnexion...")
            try:
                self.sock.send("QUIT :Test termine\r\n".encode())
                time.sleep(0.1)
            except:
                pass
            self.sock.close()
            print("[✓] Déconnecté")
    
    def run(self):
        """Point d'entrée principal"""
        if not self.connect():
            return 1
        
        if not self.authenticate():
            self.disconnect()
            return 1
        
        self.interactive_mode()
        return 0


def main():
    parser = argparse.ArgumentParser(
        description='Client de test IRC avec authentification automatique',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Exemples d'utilisation:
  %(prog)s 6667 mypass
  %(prog)s 6667 mypass -n alice
  %(prog)s 6667 mypass -n bob -u bob -r "Bob Marley"
  %(prog)s 6667 mypass -n alice -j general random
  %(prog)s 6667 mypass --host 192.168.1.10 -n testuser
        """
    )
    
    parser.add_argument('port', type=int, help='Port du serveur IRC')
    parser.add_argument('password', help='Mot de passe du serveur')
    parser.add_argument('-H', '--host', default='localhost', help='Hôte du serveur (défaut: localhost)')
    parser.add_argument('-n', '--nick', default='testuser', help='Nickname (défaut: testuser)')
    parser.add_argument('-u', '--user', help='Username (défaut: même que nickname)')
    parser.add_argument('-r', '--realname', default='Test User', help='Nom réel (défaut: Test User)')
    parser.add_argument('-j', '--join', nargs='+', metavar='CHANNEL', help='Channels à rejoindre automatiquement')
    
    args = parser.parse_args()
    
    # Si username non spécifié, utiliser le nickname
    username = args.user if args.user else args.nick
    
    client = IRCTestClient(
        host=args.host,
        port=args.port,
        password=args.password,
        nickname=args.nick,
        username=username,
        realname=args.realname,
        auto_join=args.join
    )
    
    return client.run()


if __name__ == "__main__":
    sys.exit(main())


# # Usage basique
# ./irc_test_client.py 6667 votrepassword

# # Avec un nickname personnalisé
# ./irc_test_client.py 6667 votrepassword -n alice

# # Avec auto-join de channels
# ./irc_test_client.py 6667 votrepassword -n bob -j general random help

# # Complet avec tous les paramètres
# ./irc_test_client.py 6667 votrepassword -n alice -u alice -r "Alice Cooper" -j general

# # Voir l'aide
# ./irc_test_client.py --help

# # Connexion à un serveur distant
# ./irc_test_client.py 6667 pass --host 192.168.1.10 -n testuser