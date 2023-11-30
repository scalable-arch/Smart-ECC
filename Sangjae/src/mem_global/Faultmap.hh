#ifndef __FAULTMAP_HH__
#define __FAULTMAP_HH__

#include "../fault/Packet.hh"
#include "../utils/common.hh"

#define DIMENSION 2
#define NUMLINE 128     // 1kB Cache
#define INDEX_OFFSET 7  // = log2(7)


// SRAM implementation
class FaultMap{

public:
    FaultMap();

public:
    uint8_t* getFaultMap(Packet* packet_i);
    void updateFaultMap(Packet* packet_i, int errlen_i, int* errloc_i);

protected:
    // TODO: Find the optimal hash functions
    uint8_t hashFunc1(Packet* packet_i);
    uint8_t hashFunc2(Packet* packet_i);

private:
    uint8_t* sram;
};

#endif