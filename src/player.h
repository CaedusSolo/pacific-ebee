#ifndef PLAYER_H
#define PLAYER_H

// Make name maximum to 255 charaters + null termination
typedef char PlayerName[256];

struct Player {
    PlayerName name;
    int pid;
    // Socket file descriptor
    int fd;
};

#endif // !PLAYER_H
