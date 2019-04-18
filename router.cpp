// Maksymilian Zawartko
// 289528

#include "router.h"


void UpdateRoutingInformations(RoutingTable *table) {
    for (uint32_t i = 0; i < table->size(); i++) {
        if ((*table)[i].IsReachable()) {
            if ((*table)[i].GetTurnsWithoutReception() < max_turns_without_reception &&
                    (*table)[i].GetDistance() < max_distance)
                (*table)[i].IncrementTurnsWithoutReception();
            else
                (*table)[i].SetReachable(false);
        }
        else {      // entry is not reachable
            if ((*table)[i].GetTurnsToRemove() < max_turns_to_remove)
                (*table)[i].IncrementTurnsToRemove();
            else if (!(*table)[i].IsConnectedDirectly()) {      // turns_to_remove >= max_turns_to_remove
                table->erase(table->begin() + i);
                i--;
            }
        }
    }
}

void ReceiveRoutingInformations(SocketDescriptor socket, RoutingTable *table,
                                set<string> *my_ips) {
    struct timeval turn_time;
    turn_time.tv_sec = turn_seconds;
    int ready;

    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(socket, &descriptors);

    do {
        ready = select(socket + 1, &descriptors, NULL, NULL, &turn_time);
        
        if (ready == -1) {
            cout << "Error on select\n";
        }
        // break if ready == 0
        else if (ready > 0) {
            struct sockaddr_in source;
            socklen_t source_size = sizeof(source);
            uint8_t buffer[IP_MAXPACKET];
            ssize_t packet_size = recvfrom(socket, buffer, IP_MAXPACKET, 0,
                                           (struct sockaddr *) &source, &source_size);

            if (packet_size <= 0) {
                cout << "Error on recvfrom\n";
            }
            else {
                string network_ip = ExtractIp(buffer);
                char source_ip[max_ip_length];
                inet_ntop(AF_INET, &(source.sin_addr), source_ip, max_ip_length);

                if (my_ips->find(source_ip) == my_ips->end()) {
                    uint8_t mask_length = ExtractMaskLength(buffer);
                    uint32_t distance = ExtractDistance(buffer);

                    RoutingTableEntry entry(network_ip, source_ip, mask_length, distance);
                    AddRoutingTableEntryToTable(&entry, table);
                }
            }
        }
    }
    while (ready != 0);
}

void BroadcastRoutingInformations(SocketDescriptor socket, RoutingTable *table) {
    for (auto destination = table->begin(); destination != table->end(); destination++) {
        if (destination->IsConnectedDirectly()) {
            struct sockaddr_in *recipient = destination->CreateRecipient();

            for (RoutingTableEntry entry : *table) {
                if (entry.GetTurnsToRemove() < max_turns_to_remove) {
                    uint8_t data[datagram_size];
                    entry.SaveToDatagram(data);
                    ssize_t bytes_sent = sendto(socket, data, datagram_size, 0,
                                                (struct sockaddr *) recipient, sizeof(*recipient));

                    if (bytes_sent == -1) {
                        cout << "Error on sendto\n";        // may need to be more specific
                        destination->SetReachable(false);
                        break;
                    }
                }
            }
        }
    }
}

int main() {
    int interfaces;
    cin >> interfaces;
    cin.get();      // read '\n'

    RoutingTable routing_table;
    set<string> my_ips;

    for (int i = 0; i < interfaces; i++) {
        string input_line;
        getline(cin, input_line);
        RoutingTableEntry entry(input_line);
        routing_table.push_back(entry);
        my_ips.insert(entry.GetMiddleman());
    }

    SocketDescriptor socket = CreateSocket();
    BindSocket(socket);
    int broadcast_permission = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (void *) &broadcast_permission,
                   sizeof(broadcast_permission)) == -1) {
        cout << "Error setting socket broadcast permission\n";
        exit(1);
    }

    while (true) {
        UpdateRoutingInformations(&routing_table);
        ReceiveRoutingInformations(socket, &routing_table, &my_ips);
        BroadcastRoutingInformations(socket, &routing_table);
        PrintRoutingTable(&routing_table);
    }

    return 0;
}
