#ifndef __CNN_HH__
#define __CNN_HH__

#include "../mem_global/Faultmap.hh"
#include "../fault/Packet.hh"
#include "../utils/common.hh"

class CNN{

public: 
    CNN();

public:
    double* forward(Packet* packet_i);
    void backward(Codeword label_i, double* likelihood_i);

private:
    double* weight;
    FaultMap* fm;

    // For backproagation
    uint8_t* fault_map;
    Codeword* cw;
};

#endif