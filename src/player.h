#ifndef PLAYER_H
#define PLAYER_H

#include "Connection.h"

#include <string>

using namespace std;

struct Player {
    string name;
    int pid;
    Connection* connection;
};

#endif // !PLAYER_H
