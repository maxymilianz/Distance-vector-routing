// Maksymilian Zawartko
// 289528

#include "routing_table_entry.h"


// ip <=> string
uint8_t *IpFromString(string ip) {
    unsigned int i = 0, result_index = 0;
    uint8_t *result = new uint8_t[ip_size];
    string number;

    while (i < ip.length()) {
        number += ip[i];
        i++;

        if (i >= ip.length() || ip[i] == '.') {
            result[result_index] = (uint8_t) stoi(number);
            i++;
            result_index++;
            number = "";
        }
    }

    return result;
}

string IpToString(uint8_t *ip) {
    string result = to_string(ip[0]);

    for (int i = 1; i < ip_size; i++)
        result += '.' + to_string(ip[i]);

    return result;
}

// constructors
RoutingTableEntry::RoutingTableEntry(string destination, string middleman,
                                     uint8_t mask_length, uint32_t distance) {
    this->destination = IpFromString(destination);
    this->middleman = IpFromString(middleman);
    this->mask_length = mask_length;
    this->distance = distance;
    ApplyMaskToDestination();
}

RoutingTableEntry::RoutingTableEntry(string input_line) {
    unsigned int i = 0;
    string ip;

    while (input_line[i] != '/') {
        ip += input_line[i];
        i++;
    }

    destination = IpFromString(ip);
    middleman = IpFromString(ip);

    i++;
    string number;

    while (input_line[i] != ' ') {
        number += input_line[i];
        i++;
    }

    mask_length = (uint8_t) stoi(number);
    ApplyMaskToDestination();

    i += 10;
    number = "";

    while (i < input_line.length()) {
        number += input_line[i];
        i++;
    }

    distance = (uint32_t) stoi(number);
}

// getters and setters
int RoutingTableEntry::GetTurnsToRemove() {
    return turns_to_remove;
}
void RoutingTableEntry::IncrementTurnsToRemove() {
    turns_to_remove++;
}
void RoutingTableEntry::ResetTurnsToRemove() {
    turns_to_remove = 0;
}

int RoutingTableEntry::GetTurnsWithoutReception() {
    return turns_without_reception;
}
void RoutingTableEntry::IncrementTurnsWithoutReception() {
    turns_without_reception++;
}
void RoutingTableEntry::ResetTurnsWithoutReception() {
    turns_without_reception = 0;
}

bool RoutingTableEntry::IsReachable() {
    return reachable;
}
void RoutingTableEntry::SetReachable(bool reachable) {
    this->reachable = reachable;
}

string RoutingTableEntry::GetDestination() {
    return IpToString(destination);
}

string RoutingTableEntry::GetMiddleman() {
    return IpToString(middleman);
}

uint8_t RoutingTableEntry::GetMaskLength() {
    return mask_length;
}

uint32_t RoutingTableEntry::GetDistance() {
    return reachable ? distance : UINT32_MAX;
}
void RoutingTableEntry::IncreaseDistance(uint32_t value) {
    distance += value;
}

// saving to datagram
void RoutingTableEntry::SaveToDatagram(uint8_t target[datagram_size]) {
    SaveDestinationToDatagram(target);
    SaveMaskLengthToDatagram(target + ip_size);
    SaveDistanceToDatagram(target + ip_size + mask_length_size);
}

void RoutingTableEntry::SaveDestinationToDatagram(uint8_t *target) {
    *((uint32_t *) target) = *((uint32_t *) destination);
}

void RoutingTableEntry::SaveMaskLengthToDatagram(uint8_t *target) {
    *target = mask_length;
}

void RoutingTableEntry::SaveDistanceToDatagram(uint8_t *target) {
    *((int *) target) = htonl(distance);
}

// <<
ostream &operator <<(ostream &stream, RoutingTableEntry &entry) {
    return stream << IpToString(entry.destination) << '/' << (int) entry.mask_length <<
           (entry.IsReachable() ? " distance " + to_string(entry.distance) :
                                  " unreachable") <<
           (entry.IsConnectedDirectly() ? " connected directly" :
                                        (" via " + IpToString(entry.middleman)));
}

// other
struct sockaddr_in *RoutingTableEntry::CreateRecipient() {
    struct sockaddr_in *result = new sockaddr_in;
    bzero(result, sizeof(*result));
    result->sin_family = AF_INET;
    result->sin_port = htons(port);
    inet_pton(AF_INET, IpToString(destination).c_str(), &result->sin_addr);
    return result;
}

void RoutingTableEntry::ApplyMaskToDestination() {
    *((uint32_t *) destination) = ntohl(htonl(*((uint32_t *) destination)) >>
                                        (32 - mask_length) << (32 - mask_length));
}

bool RoutingTableEntry::IsConnectedDirectly() {
    return *((uint32_t *) destination) == ntohl(htonl(*((uint32_t *) middleman)) >>
                                            (32 - mask_length) << (32 - mask_length));
}

bool RoutingTableEntry::Includes(uint8_t *ip) {
    return *((uint32_t *) destination) == ntohl(htonl(*((uint32_t *) ip)) >>
                                                (32 - mask_length) << (32 - mask_length));
}

bool RoutingTableEntry::SameDestination(RoutingTableEntry *entry) {
    return *((uint32_t *) destination) == *((uint32_t *) entry->destination);
}