#include <stdlib.h>
#include "hsiao.hh"

//--------------------------------------------------------------------
//
// WR: Encoder
// RD: Gensyndrome + Gencandidates
//
int PCM_A[] = {1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 0, 0, 0, 0, 0, 0, 0,
               1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   0, 1, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 1, 0, 0, 0, 0, 0,
               0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,
               0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 0,
               1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   0, 0, 0, 0, 0, 1, 0, 0,
               0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 0,   0, 0, 0, 0, 0, 0, 1, 0,
               0, 0, 1, 0, 0, 1, 1, 0,   0, 1, 0, 0, 1, 0, 0, 1,   1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 1, 0, 0, 1, 1,   0, 0, 0, 1, 1, 1, 0, 0,   1, 1, 1, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 1};

// PCM(Parity Check Matrix) for high speed
int PCM_L[] = {1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 0, 0,   0, 1, 1, 0, 1, 0, 0, 0,   1, 0, 0, 0, 1, 0, 0, 0,   1, 0, 0, 0, 1, 0, 0, 0,   1, 0, 0, 0, 0, 0, 0, 0,   1, 0, 0, 0, 0, 0, 0, 0,
               1, 1, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 1, 1,   0, 1, 1, 0, 0, 1, 0, 0,   0, 1, 0, 0, 0, 1, 0, 0,   0, 1, 0, 0, 0, 1, 0, 0,   0, 1, 0, 0, 0, 0, 0, 0,   0, 1, 0, 0, 0, 0, 0, 0,
               0, 0, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 1, 0,   0, 0, 1, 0, 0, 0, 1, 0,   0, 0, 1, 0, 0, 0, 1, 0,   0, 0, 1, 0, 0, 1, 1, 0,   0, 0, 1, 0, 0, 0, 0, 0,
               1, 1, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 1,   0, 0, 0, 1, 0, 0, 0, 1,   0, 0, 0, 1, 0, 0, 0, 1,   0, 0, 0, 1, 0, 1, 1, 0,   0, 0, 0, 1, 0, 0, 0, 0,
               0, 1, 1, 0, 1, 0, 0, 0,   1, 0, 0, 0, 1, 0, 0, 0,   1, 0, 0, 0, 1, 0, 0, 0,   1, 0, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 1, 1,   0, 0, 0, 0, 1, 0, 0, 0,
               0, 1, 1, 0, 0, 1, 0, 0,   0, 1, 0, 0, 0, 1, 0, 0,   0, 1, 0, 0, 0, 1, 0, 0,   0, 1, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 0, 0,   0, 0, 0, 0, 0, 1, 0, 0,
               0, 0, 0, 0, 0, 0, 1, 0,   0, 0, 1, 0, 0, 0, 1, 0,   0, 0, 1, 0, 0, 0, 1, 0,   0, 0, 1, 0, 0, 1, 1, 0,   1, 1, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 1, 0,
               0, 0, 0, 0, 0, 0, 0, 1,   0, 0, 0, 1, 0, 0, 0, 1,   0, 0, 0, 1, 0, 0, 0, 1,   0, 0, 0, 1, 0, 1, 1, 0,   0, 0, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0, 1};


Hsiao::Hsiao(int _bitN, int _bitR){
    
    bitN = _bitN;
    bitR = _bitR;

    // G matrix calculation from P matrix (tranposed)
    // G (kxn) = (Ik | P)
    //           MSB   LSB
    for (int r=bitN-bitR-1; r>=0; r--) {
        // map Parity (tranposed) matrix
        for (int c=bitN-1; c>=bitR; c--) {
            gMatrix[r][c] = ((r==(c-bitR)) ? 1 : 0);
        }
        // map indentity matrix, Ik
        for (int c=bitR-1; c>=0; c--) {
            gMatrix[r][c] = PCM_A[(bitR-1-c)*(bitN)+(bitN-bitR-1-r)];
        }
    }

    // H matrix calculation from P matrix (tranposed)
    // H (rxn) = (Pt | I(n-k))
    //           MSB      LSB
    for (int r=bitR-1; r>=0; r--) {
        for (int c=bitN-1; c>=0; c--) {
            hMatrix[r][c] = PCM_A[(bitR-1-r)*bitN+(bitN-1-c)];
        }
    }

    printhMatrix();
    verifyMatrix();
}


/**
 * @brief Simple Matrix Multiplication of GF(2)
 * 
 * The number means the position of 'AND' opeartion
 * The alphabet means the position of 'XOR' operation
 * 
 * e.g) (1 and 1a) xor (2 and 2a) xor ...(72 and 72a) --> a
 *      (1 and 1b) xor (2 and 2b) xor ...(72 and 72b) --> b
 *                      ...............
 *      (1 and 1h) xor (2 and 2h) xor ...(72 and 72h) --> h
 * 
 * input: | 1 2 3 4 5 6 7 8 ... 72 | (1x72 shape) 
 * 
 * hMatrix: | 1a 2a 3a 4a ... 72a |  (72x8 shape)
 *          | 1b 2b 3b 4b ... 72b |
 *          | 1c 2c 3c 4c ... 72c |
 *                ............
 *          | 1h 2h 3h 4h ... 72h |
 * 
 * syndrome: | a b c d e f g h |
 * 
 * @param vec_i (72bits) 
 * @return bitset<8>: : 8-bits syndrome
 */
//------------------------------------------------------------------------------
Syndrome Hsiao::genSydrome(Codeword vec_i){

    std::bitset<REDUNDANCY_LENGTH> syndrome = std::bitset<REDUNDANCY_LENGTH>(0);
    
    for(int j=0; j<bitR; j++){
        for(int i=0; i<bitN; i++){
            syndrome[j] = syndrome[j] ^ (hMatrix[j][i] && vec_i[i]);
        }   
    }

    return syndrome;    
}



/**
 * @brief Generate codeword 
 * 
 * @param data_vector (64bits)
 * @return CodewordVector 136-bit codeword vector
 */
//------------------------------------------------------------------------------
Codeword Hsiao::encode(Data data_i){
    
    Codeword encoded_codeword(0);
    for(int j=0; j<bitN-bitR; j++){
        for(int i=0; i<bitN; i++){
            encoded_codeword[i] = encoded_codeword[i] ^ (data_i[j] && gMatrix[j][i]);
        }
    }

    return encoded_codeword;
}


//------------------------------------------------------------------------------
Candidate Hsiao::getCandidate(Syndrome syn_i){

    Candidate cd;

    for(int c=0; c<bitN; c++){

        uint8_t candi_xor = 0;
        // Note that bitset[-1] is the MSB
        for(int r=REDUNDANCY_LENGTH-1; r>=0; r--){
            candi_xor = (candi_xor<<1) + hMatrix[c][r]^syn_i[r];
        }

        // Extract the position of syndrome
        uint8_t h_syn = 0;
        for(int r=REDUNDANCY_LENGTH-1; r>=0; r--){
            h_syn = (h_syn<<1) + hMatrix[c][r];
        }
        uint8_t pos0 = getPosition(Syndrome(h_syn));
        uint8_t pos1 = getPosition(Syndrome(candi_xor));

        // In case of sigle-bit error
        if(pos1==255){
            uint8_t new_pos = getPosition(syn_i);
            cd.push_back(std::make_pair(new_pos, new_pos));
        
        // double-bit error의 경우.
        }else{
            cd.push_back(std::make_pair(pos0, pos1));
        }
    }

    return cd;
}



/**
 * @brief 
 * 
 * @param element_i 
 * @return int: the position of element_i in hMatrix.
 *              Otherwise, return -1 (not exist in hMatrix)
 */
//------------------------------------------------------------------------------
uint8_t Hsiao::getPosition(Syndrome syn_i){

    for(int i=0; i<bitN; i++){
        bool is_match=true;
        for(int r=0; r<bitR; r++){
            if(syn_i[r] != hMatrix[i][r])
                is_match=false;
        }
        
        if(is_match)
            return i;
    }

    return 255;  // means no match!!
}


//------------------------------------------------------------------------------
void Hsiao::verifyMatrix()
{
    int max_row_weight = 0;
    int min_row_weight = RAND_MAX;
    for (int i=0; i<bitR; i++) {
        int row_weight = 0;
        for (int j=0; j<bitN; j++) {
            row_weight += hMatrix[i][j];
        }
        if (row_weight > max_row_weight) max_row_weight = row_weight;
        if (row_weight < min_row_weight) min_row_weight = row_weight;
    }

    // G x Ht = 0
    for (int i=0; i<bitN-bitR; i++) {
        for (int j=0; j<bitR; j++) {
            int temp = 0;
            for (int k=0; k<bitN; k++) {
                temp ^= gMatrix[i][k] & hMatrix[j][k];
            }
            if (temp!=0) {
                fprintf(stderr, "BAD H and G matrix %d\n", i);
                exit(-1);
            }
        }
    }
}


//------------------------------------------------------------------------------
void Hsiao::printhMatrix(){
    
    printf("Printing H Matrix \n");
    for (int r=bitR-1; r>=0; r--) {
        for (int c=bitN-1; c>=0; c--) {
            std::cout << static_cast<int>(hMatrix[r][c]) << " ";
        }
        std::cout << std::endl;
    }
    

    printf("Printing G Matrix \n");
    for (int r=bitN-bitR-1; r>=0; r--) {
        for (int c=bitN-1; c>=0; c--){
            std::cout << static_cast<int>(gMatrix[r][c]) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}




