#ifndef ROUTING_TABLE
#define ROUTING_TABLE

#include "routing_table_entry.h"

#include <vector>


typedef vector<RoutingTableEntry> RoutingTable;

void AddRoutingTableEntryToTable(RoutingTableEntry *entry, RoutingTable *table);

uint32_t Distance(RoutingTableEntry *destination, RoutingTable *table);

void PrintRoutingTable(RoutingTable *table);

#endif