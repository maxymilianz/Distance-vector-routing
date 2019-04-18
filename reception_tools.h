#ifndef RECEPTION_TOOLS
#define RECEPTION_TOOLS

#include "constants.h"

#include <arpa/inet.h>
#include <string>

using namespace std;


string ExtractIp(uint8_t *buffer);

uint8_t ExtractMaskLength(uint8_t *buffer);

uint32_t ExtractDistance(uint8_t *buffer);

#endif