#include "ServerConnection.h"
#include "server_game_manager.h"


using namespace std;

// Flow:
// Start server connection
// Initialize game manager with the server connection
// Accept player connections
// Fork processes once all players are connected
// Start game loop in each process

int main (int argc, char *argv[]) {
    // Default port
    int port = 8999;
    ServerConnection server(port);

    int numPlayers = 3;
    ServerGameManager gameManager(numPlayers, &server);
    gameManager.startGame();

    return 0;
}
