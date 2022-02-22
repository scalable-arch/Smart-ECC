#ifndef __HSIAO_HH__
#define __HSIAO_HH__

#include "../utils/common.hh"

class Hsiao{
    
public:
    // Constructor / destructor
    Hsiao(int _bitN, int _bitR);

    // Used in WR
    Codeword encode(Data data_i);

    // Used in RD
    Syndrome genSydrome(Codeword cw_i);
    Candidate getCandidate(Syndrome syn_i);

protected:
    uint8_t getElement(int pos);
    uint8_t getPosition(Syndrome element_i);

private:
    void verifyMatrix();
    void printhMatrix();

private:
    int bitN, bitR;

    // For test
    uint8_t hMatrix[REDUNDANCY_LENGTH][CODEWORD_LENGTH] = {0};
    uint8_t gMatrix[DATA_LENGTH][CODEWORD_LENGTH] = {0};
};

#endif /* __HSIAO_HH__ */