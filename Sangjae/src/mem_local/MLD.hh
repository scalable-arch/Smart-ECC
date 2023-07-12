#include "common.hh"

#include "../fault/Packet.hh"

class MLD{

public:
    MLD(){}

public:
    void getFloatAND(double* p, Codeword data, double* and_p);
    Codeword rankCandidate(Packet* pacekt_i, Candidate candidate_i, double* bep_i);


protected:

};