#ifndef GAME_STATES_H
#define GAME_STATES_H

enum class Ship {
    CARRIER,
    BATTLESHIP,
    CRUISER,
    DESTROYER,
    PATROL_BOAT
};

enum class GameStatus {
    ONGOING,
    WON,
    LOST
};

#endif // !GAME_STATES_H
