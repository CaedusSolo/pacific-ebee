#include "game_states.h"


// Helper function to get ship size
int get_ship_size(enum Ship ship) {
    switch(ship) {
        case CARRIER: return 5;
        case BATTLESHIP: return 4;
        case CRUISER: return 3;
        case DESTROYER: return 3;
        case PATROL_BOAT: return 2;
        default: return 0;
    }
}
