#include "messages.h"
#include <netinet/in.h>
#include <stdint.h>

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
