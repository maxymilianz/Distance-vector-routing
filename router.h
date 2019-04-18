#ifndef ROUTER
#define ROUTER

#include "reception_tools.h"
#include "routing_table.h"
#include "socket_tools.h"

#include <set>


void UpdateRoutingInformations(RoutingTable *table);

void ReceiveRoutingInformations(SocketDescriptor socket, RoutingTable *table,
                                set<string> *my_ips);

void BroadcastRoutingInformations(SocketDescriptor socket, RoutingTable *table);

int main();

#endif