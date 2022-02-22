#include "Faultmap.hh"

//------------------------------------------------------------------------------
FaultMap::FaultMap():
sram(new uint8_t[NUMLINE*CODEWORD_LENGTH]){
    std::fill(sram, sram+(NUMLINE*CODEWORD_LENGTH), 0);
}


//------------------------------------------------------------------------------
uint8_t* FaultMap::getFaultMap(Packet* packet_i){

    int index1 = hashFunc1(packet_i)*CODEWORD_LENGTH;
    int index2 = hashFunc2(packet_i)*CODEWORD_LENGTH;

    uint8_t* bitwise_min_o = new uint8_t[CODEWORD_LENGTH];

    for(int i=0; i<CODEWORD_LENGTH; i++){
        bitwise_min_o[i] = (sram[index1+i] > sram[index2+i]) ? sram[index2+i] : sram[index1+i];
    }

    return bitwise_min_o;
}


//------------------------------------------------------------------------------
void FaultMap::updateFaultMap(Packet* packet_i, int errlen_i, int* errloc_i){

    int index1 = hashFunc1(packet_i)*CODEWORD_LENGTH;
    int index2 = hashFunc2(packet_i)*CODEWORD_LENGTH;

    for(int i=0; i<errlen_i; i++){
        int loc = errloc_i[i];
        assert(loc<CODEWORD_LENGTH);
        for(int j=0; j<CODEWORD_LENGTH; j++){
            sram[index1+loc] += 1;
            sram[index2+loc] += 1;
        }
    }
}

//------------------------------------------------------------------------------
uint8_t FaultMap::hashFunc1(Packet* packet_i){
    int bank = packet_i->getBank();
    int row  = packet_i->getRow();
    int col = packet_i->getCol();
    
    int index = (row >> (ROWBIT-INDEX_OFFSET)) ^ (col >> (COLBIT-INDEX_OFFSET));

    return index;
}


//------------------------------------------------------------------------------
uint8_t FaultMap::hashFunc2(Packet* packet_i){
    int bank = packet_i->getBank();
    int row  = packet_i->getRow();
    int col = packet_i->getCol();
    
    int index = bank^(row >> (ROWBIT-INDEX_OFFSET)) ^ (col >> (COLBIT-INDEX_OFFSET));

    return index;
}


