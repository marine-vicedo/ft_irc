#include "Ircserver.hpp"
#include <string>
#include "globals.hpp"
#include <csignal>

bool g_shutdown = false;

void shutdown(int) {
    g_shutdown = true;
}
int main(int argc, char *argv[]) {
    std::signal(SIGINT, shutdown);
    signal(SIGPIPE, SIG_IGN);
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return EXIT_FAILURE;
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    IRCServer server(port, password);

    if (server.start()) {
        std::cout << "Le serveur IRC a démarré avec succès." << std::endl;
        server.acceptConnections();
    } 
    else {
        std::cerr << "Erreur lors du démarrage du serveur IRC." << std::endl;
    }
    return 0;
}