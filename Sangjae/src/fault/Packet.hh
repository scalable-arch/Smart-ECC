#ifndef __PACKET_HH__
#define __PACKET_HH__

#include "../utils/common.hh"

class Packet{

public:
    Packet(ADDR addr_i, Codeword cw_i);

public:
    /* Accessor */
    Codeword* getCodeword(){ return &cw; }
    int getBank(){ return bank; }
    int getRow(){ return row; }
    int getCol(){ return col; }

    /* Mutator */
    void setCodeword(Codeword cw_i){ cw=cw_i; }

public:
    /* Member Function */
    void injectError(int errlen_i, int* errloc_i);
    void invBit(Codeword err_mask_i){ cw = cw^err_mask_i; };
    
private:
    int bank, row, col;
    Codeword cw;

};


#endif