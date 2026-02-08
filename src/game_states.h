#ifndef GAME_STATES_H
#define GAME_STATES_H


#define ALL_SHIPS_COUNT 5

enum Ship {
    CARRIER,
    BATTLESHIP,
    CRUISER,
    DESTROYER,
    PATROL_BOAT
};

enum GameStatus {
    ONGOING,
    WON,
    LOST
};


int get_ship_size(enum Ship ship);

#endif // !GAME_STATES_H
