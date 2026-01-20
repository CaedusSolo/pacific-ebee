#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <cstdint>
#include <string>

// This pattern called CRTP (Curiously Recurring Template Pattern)
// crazy stuff
//
// For all messages to be passed through TCP, must inherit from this class
// Example in messages.h & messages.cpp
//
// This allow any class derived from Serializable to be converted to/from bytes
template<typename Derived>
class Serializable {
public:
    char* toBytes() const {
        return static_cast<const Derived*>(this)->toBytesImpl();
    }

    static Derived fromBytes(const char* data) {
        return Derived::fromBytesImpl(data);
    }
};

// === Helpers for primitive types ===

// uint32
// Can also be used for enums (with static_cast)
char* uint32ToBytes(uint32_t value);
uint32_t bytesToUint32(const char* bytes);
// string
char* stringToBytes(const std::string& str);
std::string bytesToString(const char* bytes, size_t length);


#endif // SERIALIZABLE_H
