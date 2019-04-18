#ifndef SOCKET_TOOLS
#define SOCKET_TOOLS

#include "constants.h"

#include <iostream>
#include <netinet/ip.h>
#include <strings.h>

using namespace std;


typedef int SocketDescriptor;

SocketDescriptor CreateSocket();

void BindSocket(SocketDescriptor socket);

#endif