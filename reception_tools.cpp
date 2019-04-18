// Maksymilian Zawartko
// 289528

#include "reception_tools.h"


string ExtractIp(uint8_t *buffer) {
    string result = to_string(buffer[0]);

    for (int i = 1; i < ip_size; i++)
        result += '.' + to_string(buffer[i]);

    return result;
}

uint8_t ExtractMaskLength(uint8_t *buffer) {
    return buffer[ip_size];
}

uint32_t ExtractDistance(uint8_t *buffer) {
    uint8_t result[4];

    for (int i = 0; i < 4; i++)
        result[i] = buffer[ip_size + mask_length_size + i];

    return (uint32_t) ntohl(*((uint32_t *) result));
}