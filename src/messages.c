#include "messages.h"
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


void int_serialize(char* buffer, int value) {
    uint32_t fixed_value = value;
    buffer[0] = (fixed_value >> 24) & 0xFF;
    buffer[1] = (fixed_value >> 16) & 0xFF;
    buffer[2] = (fixed_value >> 8) & 0xFF;
    buffer[3] = fixed_value & 0xFF;
}

int int_deserialize(const char* buffer) {
    return ((uint32_t)buffer[0] << 24) |
           ((uint32_t)buffer[1] << 16) |
           ((uint32_t)buffer[2] << 8) |
           ((uint32_t)buffer[3]);
}

// Serialize HitResult to buffer
// Returns number of bytes written
int hitresult_serialize(const HitResult* hr, char* buffer) {
    if (!hr || !buffer) return -1;

    char* ptr = buffer;

    // Type (4 bytes)
    *(uint32_t*)ptr = htonl((uint32_t)hr->type);
    ptr += 4;

    // Position
    *(uint32_t*)ptr = htonl(hr->position.x);
    ptr += 4;
    *(uint32_t*)ptr = htonl(hr->position.y);
    ptr += 4;

    // Ship sunk (4 bytes)
    *(uint32_t*)ptr = htonl((uint32_t)hr->ship_sunk);
    ptr += 4;

    *(uint32_t*)ptr = htonl((uint32_t)hr->attacker_index);
    ptr += 4;

    *(uint32_t*)ptr = htonl((uint32_t)hr->victim_index);
    ptr += 4;

    return 24;  // Total size
}

// Deserialize HitResult from buffer
// Returns 0 on success, -1 on error
int hitresult_deserialize(HitResult* hr, const char* buffer) {
    if (!hr || !buffer) return -1;

    const char* ptr = buffer;

    hr->type = (enum Type)ntohl(*(uint32_t*)ptr);
    ptr += 4;

    hr->position.x = ntohl(*(uint32_t*)ptr);
    ptr += 4;
    hr->position.y = ntohl(*(uint32_t*)ptr);
    ptr += 4;

    hr->ship_sunk = (enum Ship)ntohl(*(uint32_t*)ptr);
    ptr += 4;

    hr->attacker_index = ntohl(*(uint32_t*)ptr);
    ptr += 4;

    hr->victim_index = ntohl(*(uint32_t*)ptr);
    ptr += 4;

    return 0;
}
