#ifndef PLAYER_H
#define PLAYER_H


struct Player {
    // Make name maximum to 255 charaters + null termination
    char name[256];
    int pid;
    // Socket file descriptor
    int fd;
};

#endif // !PLAYER_H
