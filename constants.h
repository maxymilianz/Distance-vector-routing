#ifndef CONSTANTS
#define CONSTANTS

#include <cstdint>


const int datagram_size = 9, ip_size = 4, mask_length_size = 1;
const int port = 54321;
const int turn_seconds = 1;     // seconds, CHANGE TO 15
const int max_ip_length = 19;
const uint32_t max_distance = 32;       // for real use may need to be much greater

#endif