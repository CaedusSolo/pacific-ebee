#include "messages.h"
#include <cstring>
#include "serializable.h"


char* HitResult::toBytesImpl() const {
    // Serialize 'type'
    char* typeBytes = uint32ToBytes(static_cast<uint32_t>(type));

    // Serialize 'position'
    char* positionBytes = position.toBytes();

    // Serialize 'shipSunk'
    char* shipBytes = uint32ToBytes(static_cast<uint32_t>(shipSunk));

    // Combine all parts into a single byte array
    size_t totalSize = 4 + 8 + sizeof(Ship); // 4 bytes for type, 8 for position, rest for ship
    char* bytes = new char[totalSize];

    std::memcpy(bytes, typeBytes, 4);
    std::memcpy(bytes + 4, positionBytes, 8);
    std::memcpy(bytes + 12, shipBytes, sizeof(Ship));

    delete[] typeBytes;
    delete[] positionBytes;
    delete[] shipBytes;

    return bytes;
}

HitResult HitResult::fromBytesImpl(const char* data) {
    HitResult result;

    // Deserialize 'type'
    result.type = static_cast<Type>(bytesToUint32(data));

    // Deserialize 'position'
    result.position = Vector2D::fromBytes(data + 4);

    // Deserialize 'shipSunk'
    result.shipSunk = static_cast<Ship>(bytesToUint32(data + 12));

    return result;
}
