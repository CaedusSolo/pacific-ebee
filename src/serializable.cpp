#include "serializable.h"

#include <cstdint>
#include <cstring>
#include <arpa/inet.h> // For htonl and ntohl
#include <string>


char* uint32ToBytes(uint32_t value) {
    char* bytes = new char[4];
    value = htonl(value); // Convert to network byte order
    std::memcpy(bytes, &value, sizeof(uint32_t));

    return bytes;
}

uint32_t bytesToUint32(const char* bytes) {
    uint32_t value;
    std::memcpy(&value, bytes, sizeof(uint32_t));
    return ntohl(value); // Convert from network byte order
}

// String serialization: first 4 bytes for length, followed by string data
// Intended to hide length prefixing details from user
char* stringToBytes(const std::string& str) {
    uint32_t length = str.length();
    // Allocate extra 'sizeof(uint32_t)' bytes to store length
    char* bytes = new char[length + sizeof(uint32_t)];
    std::memcpy(bytes, uint32ToBytes(length), sizeof(uint32_t));

    std::memcpy(bytes + sizeof(uint32_t), str.c_str(), length);
    return bytes;
}

std::string bytesToString(const char* bytes) {
    uint32_t length = bytesToUint32(bytes);

    // Extract string data (excluding the length prefix)
    return std::string(bytes + sizeof(uint32_t), length);
}
