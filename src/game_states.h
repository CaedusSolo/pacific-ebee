#ifndef GAME_STATES_H
#define GAME_STATES_H

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

#endif // !GAME_STATES_H
