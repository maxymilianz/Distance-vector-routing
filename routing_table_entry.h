#ifndef ROUTING_TABLE_ENTRY
#define ROUTING_TABLE_ENTRY

#include "constants.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/ip.h>
#include <strings.h>

using namespace std;


const int max_turns_without_reception = 4, max_turns_to_remove = 4;

// ip <=> string
uint8_t *IpFromString(string ip);
string IpToString(uint8_t *ip);

class RoutingTableEntry {
    private: bool reachable = true;
    private: uint8_t mask_length;
    private: uint32_t distance;
    private: int turns_without_reception = 0, turns_to_remove = 0;
    private: uint8_t *destination, *middleman;

    // constructors
    public: RoutingTableEntry(string destination, string middleman, uint8_t mask_length,
                              uint32_t distance);
                              
    public: RoutingTableEntry(string input_line);

    // getters and setters
    public: int GetTurnsToRemove();
    public: void IncrementTurnsToRemove();
    public: void ResetTurnsToRemove();

    public: int GetTurnsWithoutReception();
    public: void IncrementTurnsWithoutReception();
    public: void ResetTurnsWithoutReception();

    public: bool IsReachable();
    public: void SetReachable(bool reachable);

    public: string GetDestination();

    public: string GetMiddleman();

    public: uint8_t GetMaskLength();

    public: uint32_t GetDistance();
    public: void IncreaseDistance(uint32_t value);

    // saving to datagram
    public: void SaveToDatagram(uint8_t target[datagram_size]);

    private: void SaveDestinationToDatagram(uint8_t *target);

    private: void SaveMaskLengthToDatagram(uint8_t *target);

    private: void SaveDistanceToDatagram(uint8_t *target);

    // <<
    friend ostream &operator <<(ostream &stream, RoutingTableEntry &entry);

    // other
    public: struct sockaddr_in *CreateRecipient();

    private: void ApplyMaskToDestination();

    public: bool IsConnectedDirectly();

    public: bool Includes(uint8_t *ip);

    public: bool SameDestination(RoutingTableEntry *entry);
};

#endif