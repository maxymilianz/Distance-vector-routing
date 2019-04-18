// Maksymilian Zawartko
// 289528

#include "routing_table.h"


void AddRoutingTableEntryToTable(RoutingTableEntry *entry, RoutingTable *table) {
    for (auto iterator = table->begin(); iterator != table->end(); iterator++) {
        if (iterator->SameDestination(entry)) {
            if (entry->IsConnectedDirectly()) {      // and iterator is connected directly
                iterator->SetReachable(true);
                iterator->ResetTurnsToRemove();
                iterator->ResetTurnsWithoutReception();
            }
            else if (!iterator->IsConnectedDirectly()) {      // and entry is not connected directly
                uint32_t distance = Distance(entry, table);

                if (distance != UINT32_MAX && (!iterator->IsReachable() ||
                        distance + entry->GetDistance() < iterator->GetDistance())) {
                    entry->IncreaseDistance(distance);
                    table->erase(iterator);
                    table->push_back(*entry);
                }
            }

            return;
        }
    }
    
    uint32_t distance = Distance(entry, table);

    if (distance != UINT32_MAX) {
        entry->IncreaseDistance(distance);
        table->push_back(*entry);
    }
}

uint32_t Distance(RoutingTableEntry *destination, RoutingTable *table) {
    for (auto iterator = table->begin(); iterator != table->end(); iterator++)
        if (iterator->Includes(IpFromString(destination->GetMiddleman()))) {
            iterator->SetReachable(true);       // reachable if received packet from it
            iterator->ResetTurnsToRemove();
            iterator->ResetTurnsWithoutReception();

            return iterator->GetDistance();
        }

    // every middleman is in a directly connected network, which is never removed
    cout << "Information about some directly connected network must have been removed\n";
    exit(1);
}

void PrintRoutingTable(RoutingTable *table) {
    cout << "Routing table:\n";
    for (RoutingTableEntry entry : *table)
        cout << entry << '\n';
    cout << '\n';
}