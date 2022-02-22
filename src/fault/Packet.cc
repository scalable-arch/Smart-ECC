#include "Packet.hh"

//------------------------------------------------------------------------------
Packet::Packet(ADDR addr_i, Codeword cw_i):
bank((addr_i&BANK_MASK)>>BANK_OFFSET),
row((addr_i&ROW_MASK)>>ROW_OFFSET),
col((addr_i&COLUM_MASK)>>COL_OFFSET),
cw(cw_i){

}


//------------------------------------------------------------------------------
void Packet::injectError(int errlen_i, int* errloc_i){    
    
    for(int i=0; i<errlen_i; i++){
        int loc = errloc_i[i];
        cw[loc] = cw[loc] ^ 1;
    }
}