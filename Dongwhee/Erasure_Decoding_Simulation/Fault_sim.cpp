#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctime>
#include<iostream>
#include<cstdlib>
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>
#include <cstring>
#include <bitset>

// Configuration 
// 필요로 하면 변경하기
#define CHANNEL_WIDTH 40
#define CHIP_NUM 10
#define DATA_CHIP_NUM 8 // sub-channel마다 data chip 개수
#define CHIP_WIDTH 4
#define BLHEIGHT 32 // RECC가 검사해야 하는 Burst length (ondie ecc의 redundancy는 제외하고 BL32만 검사한다. conservative mode 조건 고려하기)
#define SYMBOL_SIZE 8 // RECC에서 시행하는 symbol size (8이면 GF(2^8))

#define OECC_CW_LEN 136 // ondie ecc의 codeword 길이 (bit 단위)
#define OECC_DATA_LEN 128 // ondie ecc의 dataward 길이 (bit 단위)
#define OECC_REDUN_LEN 8 // ondie ecc의 redundancy 길이 (bit 단위)

#define RECC_CW_LEN 80 // rank-level ecc의 codeword 길이 (bit 단위)
#define RECC_DATA_LEN 64 // rank-level ecc의 dataward 길이 (bit 단위)
#define RECC_REDUN_LEN 16 // rank-level ecc의 redundancy 길이 (bit 단위)

#define RECC_REDUN_SYMBOL_NUM 2 // rank-level ecc의 redundancy 길이 (symbol 단위)
#define RECC_CW_SYMBOL_NUM 10 // rank-level ecc의 codeword 길이 (symbol 단위)

#define RUN_NUM 1000000 // 실행 횟수 1억번

#define CONSERVATIVE_MODE 1 // Rank-level ECC에서 Conservative mode on/off (키면 chip position도 기록해서 다른 chip correction 하면 cacheline 전체를 DUE 처리!)

//configuration over

using namespace std;
unsigned int primitive_poly[16][256]={0,}; // 16가지 primitive polynomial 각각 256개 unique 한 값들 (각 row의 맨 끝에는 0을 나타낸다.) ex : primitive_poly[4][254] = a^254, primitive_poly[4][255] = 0 (prim_num=4인 경우이고, primitive_poly = x^8+x^6+x^4+x^3+x^2+x^1+1)
unsigned int H_Matrix_SEC[OECC_REDUN_LEN][OECC_CW_LEN]; // 8 x 136

enum OECC_TYPE {NO_OECC=0, HSIAO=1}; // oecc_type
enum FAULT_TYPE {SE_NE=0, DE_NE=1, CHIPKILL_NE=2, SE_SE=3, SE_DE=4, SE_CHIPKILL=5, DE_DE=6, DE_CHIPKILL=7, CHIPKILL_CHIPKILL=8}; // fault_type
enum RECC_TYPE {AMDCHIPKILL=0, SMARTECC=1, NO_RECC=2}; // recc_type
enum RESULT_TYPE {NE=0, CE=1, DUE=2, SDC=3}; // result_type

// 지정한 정수에서, 몇번째 비트만 읽어서 반환하는 함수
int getAbit(unsigned short x, int n) { 
  return (x & (1 << n)) >> n;
}

// 다항식 형태를 10진수로 변환
unsigned int conversion_to_int_format(char *str_read, int m)
{
    unsigned int primitive_value=0;
    if(strstr(str_read,"^7")!=NULL)
        primitive_value+=int(pow(2,7));
    if(strstr(str_read,"^6")!=NULL)
        primitive_value+=int(pow(2,6));
    if(strstr(str_read,"^5")!=NULL)
        primitive_value+=int(pow(2,5));
    if(strstr(str_read,"^4")!=NULL)
        primitive_value+=int(pow(2,4));
    if(strstr(str_read,"^3")!=NULL)
        primitive_value+=int(pow(2,3));
    if(strstr(str_read,"^2")!=NULL)
        primitive_value+=int(pow(2,2));
    if(strstr(str_read,"^1+")!=NULL) // 무조건 다음에 +1은 붙기 때문!
        primitive_value+=int(pow(2,1));
    if(strstr(str_read,"+1")!=NULL)
        primitive_value+=int(pow(2,0));
    

    return primitive_value;
}

// primitive polynomial table 생성
void generate_primitive_poly(unsigned int prim_value, int m, int prim_num)
{
    unsigned int value = 0x1; // start value (0000 0001)
    int total_count = int(pow(2,m));
    int count=0;
    while (count<total_count-1){ // count : 0~254
        primitive_poly[prim_num][count]=value;
        if(value>=0x80){ // m번째 숫자가 1이면 primitive polynomial과 xor 연산
            // value의 m+1번째 숫자를 0으로 바꾸고 shift
            value=value<<(32-m+1);
            value=value>>(32-m);

            //primitive polynomial과 xor 연산
            value=value^prim_value;
        }
        else // m+1번째 숫자가 0이면 왼쪽으로 1칸 shift
            value=value<<1;
        
        count++;
    }

    return;
}

// OECC, RECC, FAULT TYPE 각각의 type을 string으로 지정. 이것을 기준으로 뒤에서 error injection, oecc, recc에서 어떻게 할지 바뀐다!!!
void oecc_recc_fault_type_assignment(string &OECC, string &FAULT, string &RECC, int *oecc_type, int *fault_type, int*recc_type, int oecc, int fault, int recc)
{
    // 1. OECC TYPE 지정
    // int oecc, int fault, int recc는 main함수 매개변수 argv로 받은 것이다. run.py에서 변경 가능
    switch (oecc){
        case NO_OECC:
            OECC.replace(OECC.begin(), OECC.end(),"RAW");
            *oecc_type=NO_OECC;
            break;
        case HSIAO:
            OECC.replace(OECC.begin(), OECC.end(),"HSIAO");
            *oecc_type=HSIAO;
            break;
        default:
            break;
    }
    switch (fault){
        case SE_NE:
            FAULT.replace(FAULT.begin(), FAULT.end(),"SE_NE");
            *fault_type=SE_NE;
            break;
        case DE_NE:
            FAULT.replace(FAULT.begin(), FAULT.end(),"DE_NE");
            *fault_type=DE_NE;
            break;
        case CHIPKILL_NE:
            FAULT.replace(FAULT.begin(), FAULT.end(),"CHIPKILL_NE");
            *fault_type=CHIPKILL_NE;
            break;
        case SE_SE:
            FAULT.replace(FAULT.begin(), FAULT.end(),"SE_SE");
            *fault_type=SE_SE;
            break;
        case SE_DE:
            FAULT.replace(FAULT.begin(), FAULT.end(),"SE_DE");
            *fault_type=SE_DE;
            break;
        case SE_CHIPKILL:
            FAULT.replace(FAULT.begin(), FAULT.end(),"SE_CHIPKILL");
            *fault_type=SE_CHIPKILL;
            break;
        case DE_DE:
            FAULT.replace(FAULT.begin(), FAULT.end(),"DE_DE");
            *fault_type=DE_DE;
            break;
        case DE_CHIPKILL:
            FAULT.replace(FAULT.begin(), FAULT.end(),"DE_CHIPKILL");
            *fault_type=DE_CHIPKILL;
            break;
        case CHIPKILL_CHIPKILL:
            FAULT.replace(FAULT.begin(), FAULT.end(),"CHIPKILL_CHIPKILL");
            *fault_type=CHIPKILL_CHIPKILL;
            break;
        default:
            break;
    }
    switch (recc){
        // (80,64) SSC [8b symbol]
        case AMDCHIPKILL: 
            RECC.replace(RECC.begin(), RECC.end(),"AMDCHIPKILL");
            *recc_type=AMDCHIPKILL;
            break;
        // (80, 64) Smart ECC (8b symbol)
        // Error/Erasure decoding
        case SMARTECC:
            RECC.replace(RECC.begin(), RECC.end(),"SMARTECC");
            *recc_type=SMARTECC;
            break;
        case NO_RECC:
            RECC.replace(RECC.begin(), RECC.end(),"RAW");
            *recc_type=NO_RECC;
            break;
        default:
            break;
    }
    return;
}

// SE injection (Single Error injection)
void error_injection_SE(int Fault_Chip_position, unsigned int Chip_array[][OECC_CW_LEN], int oecc_type)
{
    int Fault_pos;
    if(oecc_type==NO_OECC){
        Fault_pos = rand()%OECC_DATA_LEN; // 0~127
    }
    else if(oecc_type==HSIAO){
        Fault_pos = rand()%OECC_CW_LEN; // 0~135
    }

    Chip_array[Fault_Chip_position][Fault_pos]^=1;
    return;
}

// DE injection (Double Error injection)
void error_injection_DE(int Fault_Chip_position, unsigned int Chip_array[][OECC_CW_LEN], int oecc_type)
{
    int First_fault_pos;
    int Second_fault_pos;
    if(oecc_type==NO_OECC){
        First_fault_pos = rand()%OECC_DATA_LEN; // 0~127
    }
    else if(oecc_type==HSIAO){
        First_fault_pos = rand()%OECC_CW_LEN; // 0~135
    }

    while(1){
        if(oecc_type==NO_OECC){
            Second_fault_pos = rand()%OECC_DATA_LEN; // 0~127
        }
        else if(oecc_type==HSIAO){
            Second_fault_pos = rand()%OECC_CW_LEN; // 0~135
        }
        if(First_fault_pos!=Second_fault_pos)
            break;
    }

    Chip_array[Fault_Chip_position][First_fault_pos]^=1;
    Chip_array[Fault_Chip_position][Second_fault_pos]^=1;
    return;
}

// Chipkill injection
void error_injection_CHIPKILL(int Fault_Chip_position, unsigned int Chip_array[][OECC_CW_LEN], int oecc_type)
{
    if(oecc_type==NO_OECC){
        for(int Fault_pos=0; Fault_pos<OECC_DATA_LEN; Fault_pos++){ // 0~127
            if(rand()%2!=0) // 0(no error) 'or' 1(bit-flip)
                Chip_array[Fault_Chip_position][Fault_pos]^=1;
        }
    }
    else if(oecc_type==HSIAO){
        for(int Fault_pos=0; Fault_pos<OECC_CW_LEN; Fault_pos++){ // 0~135
            if(rand()%2!=0) // 0(no error) 'or' 1(bit-flip)
                Chip_array[Fault_Chip_position][Fault_pos]^=1;
        }
    }

    return;
}

// OECC 1bit correction
int error_correction_oecc(int Fault_Chip_position, unsigned int Chip_array[][OECC_CW_LEN])
{
    unsigned int Syndromes[OECC_REDUN_LEN]; // 8 x 1
    
    // Syndromes = H * C^T
    for(int row=0; row<OECC_REDUN_LEN; row++){
        unsigned int row_value=0;
        for(int column=0; column<OECC_CW_LEN; column++)
            row_value=row_value^(H_Matrix_SEC[row][column] * Chip_array[Fault_Chip_position][column]);
        Syndromes[row]=row_value;
    }

    // NE 검사 (또는 SDC 발생 가능)
    int cnt=0;
    for(int index=0; index<OECC_REDUN_LEN; index++){
        if(Syndromes[index]==1)
            cnt++;
    }
    if(cnt==0) // Syndrome all zero
        return NE;

    // CE 또는 DUE 또는 SDC
    // error correction (Check Syndromes)
    cnt=0;
    for(int error_pos=0; error_pos<OECC_CW_LEN; error_pos++){
        cnt=0;
        for(int row=0; row<OECC_REDUN_LEN; row++){
            if(Syndromes[row]==H_Matrix_SEC[row][error_pos])
                cnt++;
            else
                break;
        }
        // 1-bit error 일때만 error correction 진행
        if(cnt==OECC_REDUN_LEN){
            Chip_array[Fault_Chip_position][error_pos]^=1; // error correction (bit-flip)
            return CE;
        }
    }

    // Error가 발생했지만 1-bit error는 아닌 경우이다.
    // 이 경우에는 correction을 진행하지 않는다.
    return DUE;
}

int SDC_check(int BL, unsigned int Chip_array[][OECC_CW_LEN], int oecc_type, int recc_type)
{
    // Cacheline 에서 1이 남아있는지 검사
    // -> RECC 있으면 chip 0~9까지, RECC 없으면 chip 0~7까지

    int error_check=0;
    
    // RECC 없는 경우
    if(recc_type==NO_RECC){
        for(int Error_chip_pos=0; Error_chip_pos<DATA_CHIP_NUM; Error_chip_pos++){ // 0~7번쨰 chip까지
            for(int Fault_pos=BL*4; Fault_pos<(BL+2)*4; Fault_pos++){ // 0~127b 까지
                if(Chip_array[Error_chip_pos][Fault_pos]==1){
                    error_check++;
                    return error_check;
                } 
            }
        }
    }
    
    // RECC 있는 경우
    else if(recc_type!=NO_RECC){
        for(int Error_chip_pos=0; Error_chip_pos<CHIP_NUM; Error_chip_pos++){ // 0~9번쨰 chip까지
            for(int Fault_pos=BL*4; Fault_pos<(BL+2)*4; Fault_pos++){ // 0~127b 까지
                if(Chip_array[Error_chip_pos][Fault_pos]==1){
                    error_check++;
                    return error_check;
                }
            }
        }
    }

    return error_check;
}

int Yield_check(unsigned int Chip_array[][OECC_CW_LEN], int oecc_type, int recc_type)
{
    // Cacheline 2개 전체에서 1이 남아있는지 검사
    // -> RECC 있으면 chip 0~9까지, RECC 없으면 chip 0~7까지

    int error_check=0;
 
    // RECC 없는 경우   
    if(recc_type==NO_RECC){
        for(int Error_chip_pos=0; Error_chip_pos<DATA_CHIP_NUM; Error_chip_pos++){ // 0~7번쨰 chip까지
            for(int Fault_pos=0; Fault_pos<OECC_DATA_LEN; Fault_pos++){ // 0~127b 까지
                if(Chip_array[Error_chip_pos][Fault_pos]==1){
                    error_check++;
                    return error_check;
                }
            }
        }
    }
    
    // OECC 없고, RECC 있는 경우
    else if(recc_type!=NO_RECC){
        for(int Error_chip_pos=0; Error_chip_pos<CHIP_NUM; Error_chip_pos++){ // 0~9번쨰 chip까지
            for(int Fault_pos=0; Fault_pos<OECC_DATA_LEN; Fault_pos++){ // 0~127b 까지
                if(Chip_array[Error_chip_pos][Fault_pos]==1){
                    error_check++;
                    return error_check;
                }
            }
        }
    }

    return error_check;
}

// AMDCHIPKILL
// 여기는 configuration 자동화 안 시켰음 (나중에 channel size, BL등을 바꿀거면 여기는 주의해서 보기!!)
void error_correction_recc_AMDCHIPKILL(int BL, int *result_type_recc, set<int> &error_chip_position, unsigned int Chip_array[][OECC_CW_LEN])
{
    // primitive polynomial : x^8+x^4+x^3+x^2+1
    /*
        (1) Syndrome이 모두 0이면 => return NE
        (2) Syndrome이 0이 아니고, SSC의 syndrome과 일치하면 SSC 진행 [S1/S0이 a^0~a^9중 하나] => return CE
        (3) Syndrome이 0이 아니고, SSC의 syndrome과 일치하지 않으면 => return DUE
    */
    
    // codeword 생성
    unsigned int codeword[RECC_CW_LEN];
    for(int column=0; column<CHIP_NUM; column++){ // 0~9
        //printf("chip index : %d,",column/8);
        for(int symbol_index=BL*4; symbol_index<(BL+2)*4; symbol_index++){ // 0~7, 8~15, 16~23 ... 
            int chip_index=column; // 0~9
            //printf("symbol_index : %d, ",symbol_index);
            codeword[chip_index*8+(symbol_index)%8]=Chip_array[chip_index][symbol_index];
        }
        //printf("\n");
    }

    //printf("\ncodeword : ");
    //for(int column=0; column<RECC_CW_LEN; column++)
    //    printf("%d ",codeword[column]);
    //printf("\n");

    // Syndrome 계산
    // S0 = (a^exponent0) ^ (a^exponent1) ^ (a^exponent2) ... ^(a^exponent9)
    // S1 = (a^exponent0) ^ (a^[exponent1+1]) ^ (a^[exponent2+2]) ... ^ (a^[exponent9+9])
    // S0 계산
    unsigned int S0=0,S1=0;
    for(int symbol_index=0; symbol_index<RECC_CW_SYMBOL_NUM; symbol_index++){ // 0~9
        unsigned exponent=255; // 0000_0000이면 255 (해당 사항만 예외케이스!)
        unsigned symbol_value=0; // 0000_0000 ~ 1111_1111
        // ex : codeword의 첫 8개 bit가 0 1 0 1 1 1 0 0 이면
        // symbol_value는 (0<<7) ^ (1<<6) ^ (0<<5) ^ (1<<4) ^ (1<<3) ^ (1<<2) ^ (0<<1) ^ (0<<0) = 0101_1100 이다.
        for(int symbol_value_index=0; symbol_value_index<SYMBOL_SIZE; symbol_value_index++){ // 8-bit symbol
            symbol_value^=(codeword[symbol_index*8+symbol_value_index] << (SYMBOL_SIZE-1-symbol_value_index)); // <<7, <<6, ... <<0
        }
        for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
            if(symbol_value==primitive_poly[0][prim_exponent]){
                exponent=prim_exponent;
                break;
            }
        }
        //printf("symbol_index : %d, symbol_value : %d\n",symbol_index, symbol_value);

        if(exponent!=255) // S0 = (a^exponent0) ^ (a^exponent1) ^ (a^exponent2) ... ^(a^exponent9)
            S0^=primitive_poly[0][exponent];
    }


    // S1 계산
    for(int symbol_index=0; symbol_index<RECC_CW_SYMBOL_NUM; symbol_index++){ // 0~9
        unsigned exponent=255; // 0000_0000이면 255 (해당 사항만 예외케이스!)
        unsigned symbol_value=0; // 0000_0000 ~ 1111_1111
        for(int symbol_value_index=0; symbol_value_index<SYMBOL_SIZE; symbol_value_index++){ // 8-bit symbol
            symbol_value^=(codeword[symbol_index*8+symbol_value_index] << (SYMBOL_SIZE-1-symbol_value_index)); // <<7, <<6, ... <<0
        }
        for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
            if(symbol_value==primitive_poly[0][prim_exponent]){
                exponent=prim_exponent;
                break;
            }
        }
        
        if(exponent!=255) // S1 = (a^exponent0) ^ (a^[exponent1+1]) ^ (a^[exponent2+2]) ... ^ (a^[exponent9+9])
            S1^=primitive_poly[0][(exponent+symbol_index)%255];
    }

    // S0 = a^p, S1= a^q (a^0 ~ a^254)
    unsigned int p,q;
    for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
        if(S0==primitive_poly[0][prim_exponent])
            p=prim_exponent;
        if(S1==primitive_poly[0][prim_exponent])
            q=prim_exponent;
    }
    //printf("S0 : %d(a^%d), S1 : %d(a^%d)\n",S0,p,S1,q);

    //printf("S0 : %d\n",S0);
    if(S0==0 && S1==0){ // NE (No Error)
        *result_type_recc=NE;
        return;
    }
    
    // CE 'or' DUE
    // error chip position
    int error_chip_position_recc;
    error_chip_position_recc=(q+255-p)%255;

    // Table
    if(0<=error_chip_position_recc && error_chip_position_recc < CHIP_NUM){ // CE (error chip location : 0~9)
        // printf("CE case! error correction start!\n");
        //error correction
        for(int symbol_index=0; symbol_index<SYMBOL_SIZE; symbol_index++){ // 0~7
            Chip_array[error_chip_position_recc][BL*4+symbol_index]^=getAbit(S0, SYMBOL_SIZE-1-symbol_index); // S0 >> 7, S0 >> 6 ... S0 >> 0
        }
        // printf("CE case! error correction done!\n");     

        // return result type recc (CE), error chip position
        error_chip_position.insert(error_chip_position_recc);
        *result_type_recc=CE;
        return;
    }
    // Table End!!!!!
    
    // DUE
    // 신드롬이 0이 아니고, correction 진행 안한 경우
    *result_type_recc=DUE;
    
    return;
}

// SMARTECC (Error correction)
// DUE 개수가  2개 미만일때 실행
// 여기는 configuration 자동화 안 시켰음 (나중에 channel size, BL등을 바꿀거면 여기는 주의해서 보기!!)
void error_correction_recc_SMARTECC(int BL, int *result_type_recc, set<int> &error_chip_position, unsigned int Chip_array[][OECC_CW_LEN])
{
    // primitive polynomial : x^8+x^4+x^3+x^2+1
    /*
        DUE 개수 : 1개 이하 -> error correction (Single Symbol Correction) -> 1 chip error 100% correction    
        
        (1) Syndrome이 모두 0이면 => return NE
        (2) Syndrome이 0이 아니고, SSC의 syndrome과 일치하면 SSC 진행 [S1/S0이 a^0~a^9중 하나] => return CE
        (3) Syndrome이 0이 아니고, SSC의 syndrome과 일치하지 않으면 => return DUE


    */
    
    // codeword 생성
    unsigned int codeword[RECC_CW_LEN];
    for(int column=0; column<CHIP_NUM; column++){ // 0~9
        //printf("chip index : %d,",column/8);
        for(int symbol_index=BL*4; symbol_index<(BL+2)*4; symbol_index++){ // 0~7, 8~15, 16~23 ... 
            int chip_index=column; // 0~9
            //printf("symbol_index : %d, ",symbol_index);
            codeword[chip_index*8+(symbol_index)%8]=Chip_array[chip_index][symbol_index];
        }
        //printf("\n");
    }

    //printf("\ncodeword : ");
    //for(int column=0; column<RECC_CW_LEN; column++)
    //    printf("%d ",codeword[column]);
    //printf("\n");

    // Syndrome 계산
    // S0 = (a^exponent0) ^ (a^exponent1) ^ (a^exponent2) ... ^(a^exponent9)
    // S1 = (a^exponent0) ^ (a^[exponent1+1]) ^ (a^[exponent2+2]) ... ^ (a^[exponent9+9])
    // S0 계산
    unsigned int S0=0,S1=0;
    for(int symbol_index=0; symbol_index<RECC_CW_SYMBOL_NUM; symbol_index++){ // 0~9
        unsigned exponent=255; // 0000_0000이면 255 (해당 사항만 예외케이스!)
        unsigned symbol_value=0; // 0000_0000 ~ 1111_1111
        // ex : codeword의 첫 8개 bit가 0 1 0 1 1 1 0 0 이면
        // symbol_value는 (0<<7) ^ (1<<6) ^ (0<<5) ^ (1<<4) ^ (1<<3) ^ (1<<2) ^ (0<<1) ^ (0<<0) = 0101_1100 이다.
        for(int symbol_value_index=0; symbol_value_index<SYMBOL_SIZE; symbol_value_index++){ // 8-bit symbol
            symbol_value^=(codeword[symbol_index*8+symbol_value_index] << (SYMBOL_SIZE-1-symbol_value_index)); // <<7, <<6, ... <<0
        }
        for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
            if(symbol_value==primitive_poly[0][prim_exponent]){
                exponent=prim_exponent;
                break;
            }
        }
        //printf("symbol_index : %d, symbol_value : %d\n",symbol_index, symbol_value);

        if(exponent!=255) // S0 = (a^exponent0) ^ (a^exponent1) ^ (a^exponent2) ... ^(a^exponent9)
            S0^=primitive_poly[0][exponent];
    }


    // S1 계산
    for(int symbol_index=0; symbol_index<RECC_CW_SYMBOL_NUM; symbol_index++){ // 0~9
        unsigned exponent=255; // 0000_0000이면 255 (해당 사항만 예외케이스!)
        unsigned symbol_value=0; // 0000_0000 ~ 1111_1111
        for(int symbol_value_index=0; symbol_value_index<SYMBOL_SIZE; symbol_value_index++){ // 8-bit symbol
            symbol_value^=(codeword[symbol_index*8+symbol_value_index] << (SYMBOL_SIZE-1-symbol_value_index)); // <<7, <<6, ... <<0
        }
        for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
            if(symbol_value==primitive_poly[0][prim_exponent]){
                exponent=prim_exponent;
                break;
            }
        }
        
        if(exponent!=255) // S1 = (a^exponent0) ^ (a^[exponent1+1]) ^ (a^[exponent2+2]) ... ^ (a^[exponent9+9])
            S1^=primitive_poly[0][(exponent+symbol_index)%255];
    }

    // S0 = a^p, S1= a^q (a^0 ~ a^254)
    unsigned int p,q;
    for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
        if(S0==primitive_poly[0][prim_exponent])
            p=prim_exponent;
        if(S1==primitive_poly[0][prim_exponent])
            q=prim_exponent;
    }
    //printf("S0 : %d(a^%d), S1 : %d(a^%d)\n",S0,p,S1,q);

    //printf("S0 : %d\n",S0);
    if(S0==0 && S1==0){ // NE (No Error)
        *result_type_recc=NE;
        return;
    }
    
    // CE 'or' DUE
    // error chip position
    int error_chip_position_recc;
    error_chip_position_recc=(q+255-p)%255;

    // Table
    if(0<=error_chip_position_recc && error_chip_position_recc < CHIP_NUM){ // CE (error chip location : 0~9)
        // printf("CE case! error correction start!\n");
        //error correction
        for(int symbol_index=0; symbol_index<SYMBOL_SIZE; symbol_index++){ // 0~7
            Chip_array[error_chip_position_recc][BL*4+symbol_index]^=getAbit(S0, SYMBOL_SIZE-1-symbol_index); // S0 >> 7, S0 >> 6 ... S0 >> 0
        }
        // printf("CE case! error correction done!\n");     

        // return result type recc (CE), error chip position
        error_chip_position.insert(error_chip_position_recc);
        *result_type_recc=CE;
        return;
    }
    // Table End!!!!!
    
    // DUE
    // 신드롬이 0이 아니고, correction 진행 안한 경우
    *result_type_recc=DUE;
    
    return;
}

// SMARTECC (Erasure correction)
// DUE 개수가  2개 일때 실행
// 여기는 configuration 자동화 안 시켰음 (나중에 channel size, BL등을 바꿀거면 여기는 주의해서 보기!!)
void erasure_correction_recc_SMARTECC(int BL, int *result_type_recc, set<int> &error_chip_position, unsigned int Chip_array[][OECC_CW_LEN], int first_error_chip_position, int second_error_chip_position)
{
    // primitive polynomial : x^8+x^4+x^3+x^2+1
    /*
        DUE 개수 : 2개 -> erasure correction (Double Symbol Correction) -> 2 chip error 100% correction (DUE가 잘 나온다는 가정하에)
             
        (1) Syndrome이 모두 0이면 => return NE
        (2) Syndrome이 0이 아니면, double symbol correction 진행
    */

    // codeword 생성
    unsigned int codeword[RECC_CW_LEN];
    for(int column=0; column<CHIP_NUM; column++){ // 0~9
        //printf("chip index : %d,",column/8);
        for(int symbol_index=BL*4; symbol_index<(BL+2)*4; symbol_index++){ // 0~7, 8~15, 16~23 ... 
            int chip_index=column; // 0~9
            //printf("symbol_index : %d, ",symbol_index);
            codeword[chip_index*8+(symbol_index)%8]=Chip_array[chip_index][symbol_index];
        }
        //printf("\n");
    }

    //printf("\ncodeword : ");
    //for(int column=0; column<RECC_CW_LEN; column++)
    //    printf("%d ",codeword[column]);
    //printf("\n");

    // Syndrome 계산
    // S0 = (a^exponent0) ^ (a^exponent1) ^ (a^exponent2) ... ^(a^exponent9)
    // S1 = (a^exponent0) ^ (a^[exponent1+1]) ^ (a^[exponent2+2]) ... ^ (a^[exponent9+9])
    // S0 계산
    unsigned int S0=0,S1=0;
    for(int symbol_index=0; symbol_index<RECC_CW_SYMBOL_NUM; symbol_index++){ // 0~9
        unsigned int exponent=255; // 0000_0000이면 255 (해당 사항만 예외케이스!)
        unsigned int symbol_value=0; // 0000_0000 ~ 1111_1111
        // ex : codeword의 첫 8개 bit가 0 1 0 1 1 1 0 0 이면
        // symbol_value는 (0<<7) ^ (1<<6) ^ (0<<5) ^ (1<<4) ^ (1<<3) ^ (1<<2) ^ (0<<1) ^ (0<<0) = 0101_1100 이다.
        for(int symbol_value_index=0; symbol_value_index<SYMBOL_SIZE; symbol_value_index++){ // 8-bit symbol
            symbol_value^=(codeword[symbol_index*8+symbol_value_index] << (SYMBOL_SIZE-1-symbol_value_index)); // <<7, <<6, ... <<0
        }
        for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
            if(symbol_value==primitive_poly[0][prim_exponent]){
                exponent=prim_exponent;
                break;
            }
        }
        //printf("symbol_index : %d, symbol_value : %d\n",symbol_index, symbol_value);

        if(exponent!=255) // S0 = (a^exponent0) ^ (a^exponent1) ^ (a^exponent2) ... ^(a^exponent9)
            S0^=primitive_poly[0][exponent];
    }


    // S1 계산
    for(int symbol_index=0; symbol_index<RECC_CW_SYMBOL_NUM; symbol_index++){ // 0~9
        unsigned int exponent=255; // 0000_0000이면 255 (해당 사항만 예외케이스!)
        unsigned int symbol_value=0; // 0000_0000 ~ 1111_1111
        for(int symbol_value_index=0; symbol_value_index<SYMBOL_SIZE; symbol_value_index++){ // 8-bit symbol
            symbol_value^=(codeword[symbol_index*8+symbol_value_index] << (SYMBOL_SIZE-1-symbol_value_index)); // <<7, <<6, ... <<0
        }
        for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
            if(symbol_value==primitive_poly[0][prim_exponent]){
                exponent=prim_exponent;
                break;
            }
        }
        
        if(exponent!=255) // S1 = (a^exponent0) ^ (a^[exponent1+1]) ^ (a^[exponent2+2]) ... ^ (a^[exponent9+9])
            S1^=primitive_poly[0][(exponent+symbol_index)%255];
    }

    // S0 = a^p, S1= a^q (a^0 ~ a^254)
    unsigned int p=255,q=255;
    for(int prim_exponent=0; prim_exponent<255; prim_exponent++){
        if(S0==primitive_poly[0][prim_exponent])
            p=prim_exponent;
        if(S1==primitive_poly[0][prim_exponent])
            q=prim_exponent;
    }
    //printf("S0 : %d(a^%d), S1 : %d(a^%d)\n",S0,p,S1,q);

    getchar();

    //printf("S0 : %d\n",S0);
    // NE (No Error)
    if(S0==0 && S1==0){
        //printf("NE cases!\n");
        *result_type_recc=NE;
        return;
    }

    // 2 symbol error
    // first_error chip position=i
    // second_error chip position=j
    //printf("2 symbol error cases!\n");

    // first error value (a^n)
    // S0 x a^j + S1 = a^n(a^i + a^j)
    // a^n = (S0 x a^j + S1)/(a^i + a^j)

    /*
    printf("before codeword: ");
    for(int index=0; index<RECC_CW_LEN; index++){
        if(index%8==0 && index>0)
            printf(" ");
        printf("%d", codeword[index]);
    }
    printf("\n");
    */

    // S0==0 && S1!=0
    if(S0==0 && S1!=0){
        unsigned int symbol_value=(primitive_poly[0][first_error_chip_position]) ^ (primitive_poly[0][second_error_chip_position]); // a^i + a^j
        unsigned int exponent=0;
        for(int exponent_index=0; exponent_index<256; exponent_index++){
            if(primitive_poly[0][exponent_index]==symbol_value){
                exponent=exponent_index; // a^i + a^j = a^? -> ? 구하기
                break;
            }
        }
        unsigned int first_error_value=(primitive_poly[0][(q+255-exponent)%255]);
        unsigned int second_error_value=first_error_value;

        for(int symbol_index=0; symbol_index<SYMBOL_SIZE; symbol_index++){ // 0~7
            Chip_array[first_error_chip_position][BL*4+symbol_index]^=getAbit(first_error_value, SYMBOL_SIZE-1-symbol_index); // S0 >> 7, S0 >> 6 ... S0 >> 0
            Chip_array[second_error_chip_position][BL*4+symbol_index]^=getAbit(second_error_value, SYMBOL_SIZE-1-symbol_index); // S0 >> 7, S0 >> 6 ... S0 >> 0
        }
        *result_type_recc=CE;
        return;
    }

    

    //printf("first error value calculation!!\n");
    unsigned int symbol_value=(primitive_poly[0][(p+second_error_chip_position)%255]) ^ S1; // S0 x a^j + S1
    unsigned int symbol_value2=(primitive_poly[0][first_error_chip_position]) ^ (primitive_poly[0][second_error_chip_position]); // a^i + a^j
    //printf("symbol value: %d, symbol value2: %d\n",symbol_value, symbol_value2);
    unsigned int exponent=0, exponent2=0;
    for(int exponent_index=0; exponent_index<256; exponent_index++){
        if(primitive_poly[0][exponent_index]==symbol_value){
            exponent=exponent_index; // S0 x a^j + S1 = a^? -> ? 구하기
        }
        if(primitive_poly[0][exponent_index]==symbol_value2){
            exponent2=exponent_index; // a^i + a^j = a^? -> ? 구하기
        }
    }

    //cout << "a^i + a^j = a^( "<< exponent2 << "): " << bitset<8>(symbol_value2) << endl;
    
    unsigned int first_error_value, second_error_value;
    if(exponent==255) // S0 ==0
        first_error_value=0;
    else
        first_error_value=primitive_poly[0][(exponent+255-exponent2)%255];
        
    second_error_value=S0^first_error_value;

    // second error value (a^m)
    // a^m = S0 + a^n
    //printf("second error value calculation!!\n");

    // error correction
    //printf("\nerror chip position : %d, %d\n",first_error_chip_position,second_error_chip_position);
    //cout <<  "first error value: " << bitset<8>(first_error_value) << endl;
    //cout <<  "second error value: " << bitset<8>(second_error_value) << endl;
        
    //printf("2 symbol error correction!!\n");
    for(int symbol_index=0; symbol_index<SYMBOL_SIZE; symbol_index++){ // 0~7
        Chip_array[first_error_chip_position][BL*4+symbol_index]^=getAbit(first_error_value, SYMBOL_SIZE-1-symbol_index); // S0 >> 7, S0 >> 6 ... S0 >> 0
        Chip_array[second_error_chip_position][BL*4+symbol_index]^=getAbit(second_error_value, SYMBOL_SIZE-1-symbol_index); // S0 >> 7, S0 >> 6 ... S0 >> 0
    }

    /*
    printf("after codeword: ");
    for(int column=0; column<CHIP_NUM; column++){ // 0~9
    //printf("chip index : %d,",column/8);
        for(int symbol_index=BL*4; symbol_index<(BL+2)*4; symbol_index++){ // 0~7, 8~15, 16~23 ... 
            int chip_index=column; // 0~9
            //printf("symbol_index : %d, ",symbol_index);
            printf("%d",Chip_array[chip_index][symbol_index]);
        }
        printf(" ");
    //printf("\n");
    }
    printf("\n\n");
    */
   
    //printf("2 symbol error cases!\n");
    *result_type_recc=CE;
    return;
}

int main(int argc, char* argv[])
{
    // 1. GF(2^8) primitive polynomial table 생성
    // prim_num으로 구분한다!!!!!!!!!!!!!!!!!
    FILE *fp=fopen("GF_2^8__primitive_polynomial.txt","r");
    int primitive_count=0;
    while(1){
        char str_read[100];
        unsigned int primitive_value=0;
        fgets(str_read,100,fp);
        primitive_value=conversion_to_int_format(str_read, 8);

        generate_primitive_poly(primitive_value,8,primitive_count); // ex : primitive polynomial : a^16 = a^9+a^8+a^7+a^6+a^4+a^3+a^2+1 = 0000 0011 1101 1101 = 0x03DD (O) -> 맨 오른쪽 prim_num : 0
        primitive_count++;

        if(feof(fp))
            break;
    }
    fclose(fp);

    // 2. H_Matrix 설정
    // SEC : OECC
    FILE *fp4=fopen("H_Matrix_SEC.txt","r");
    while(1){
        unsigned int value;
        for(int row=0; row<OECC_REDUN_LEN; row++){
            for(int column=0; column<OECC_CW_LEN; column++){
                fscanf(fp4,"%d ",&value);
                H_Matrix_SEC[row][column]=value;
                //printf("%d ",H_Matrix_binary[row][column]);
            }
        }
        if(feof(fp4))
            break;
    }
    fclose(fp4);

    // 3. 출력 파일 이름 설정 & oecc/fault/recc type 설정 (main 함수의 argv parameter로 받는다.
    // run.py에서 변경 가능!!!

    // 파일명 예시
    // ex : HSIAO_AMDCHIPKILL_SE_NE -> OECC에는 HSIAO(SEC), RECC에는 AMDCHIPKILL, FAULT는 1개 chip에만 1bit(SE[Single Error]) 발생하고 나머지 chip에는 error가 발생하지 않는(NE[No Error]) 경우
    // ex : RAW_SMARTECC_CHIPKILL_SE -> OECC는 없다 (RAW), RECC에는 SMART ECC, FAULT는 2개 chip에 error가 발생하고 (1개는 chipkill, 1개는 SE) 나머지는 NE 이다.
    string OECC="X", RECC="X", FAULT="X"; // => 파일 이름 생성을 위한 변수들. 그 이후로는 안쓰인다.
    int oecc_type, recc_type,fault_type; // => on-die ECC, Rank-level ECC, fault_type 분류를 위해 쓰이는 변수. 뒤에서도 계속 사용된다.
    oecc_recc_fault_type_assignment(OECC, FAULT, RECC, &oecc_type, &fault_type, &recc_type, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    
    string Result_file_name = OECC + "_" + RECC + "_" + FAULT + ".S";
    FILE *fp3=fopen(Result_file_name.c_str(),"w"); // c_str : string class에서 담고 있는 문자열을 c에서의 const char* 타입으로 변환하여 반환해주는 편리한 멤버함수

    // 4. 여기서부터 반복문 시작 (1억번)
    // DIMM 설정 (Channel에 있는 chip 구성을 기본으로 한다.)
    // DDR4 : x4 chip 기준으로 18개 chip이 있다. 각 chip은 on-die ECC codeword 136b이 있다.
    // DDR5 : x4 chip 기준으로 10개 chip이 있다. 각 chip은 on-die ECC codeword 136b이 있다.

    unsigned int Chip_array[CHIP_NUM][OECC_CW_LEN]; // 전체 chip 구성 (BL34 기준. [data : BL32, OECC-redundancy : BL2])
    int CE_cnt=0, DUE_cnt=0, SDC_cnt=0; // CE, DUE, SDC 횟수
    srand((unsigned int)time(NULL)); // 난수 시드값 계속 변화
    for(int runtime=0; runtime<RUN_NUM; runtime++){
        if(runtime%1000000==0){
            fprintf(fp3,"\n===============\n");
            fprintf(fp3,"Runtime : %d/%d\n",runtime,RUN_NUM);
            fprintf(fp3,"CE : %d\n",CE_cnt);
            fprintf(fp3,"DUE : %d\n",DUE_cnt);
            fprintf(fp3,"SDC : %d\n",SDC_cnt);
            fprintf(fp3,"\n===============\n");
	    fflush(fp3);
        }
        // 4-1. 10개 chip의 136b 전부를 0으로 초기화 (no-error)
        // 이렇게 하면 굳이 encoding을 안해도 된다. no-error라면 syndrome이 0으로 나오기 때문!
        for(int i=0; i<CHIP_NUM; i++)
            memset(Chip_array[i], 0, sizeof(unsigned int) * OECC_CW_LEN); 

        // 4-2. Error injection
        // [1] 3개의 chip을 선택 (Fault_Chip_position)
        // [2] error injection (SE, CHIPKILL, NE) [각각 다른 위치의 chip 3개 뽑기 (0~9)]
        // [3] chipkill은 각 bit마다 50% 확률로 bit/flip 발생
        vector<int> Fault_Chip_position;
        for (;;) {
            Fault_Chip_position.clear();
            if(recc_type==NO_RECC){
                Fault_Chip_position.push_back(rand()%DATA_CHIP_NUM); // Fault_Chip_position[0] // 0~7
                Fault_Chip_position.push_back(rand()%DATA_CHIP_NUM); // Fault_Chip_position[1] // 0~7
                }
            else{
                Fault_Chip_position.push_back(rand()%CHIP_NUM); // Fault_Chip_position[0] // 0~9
                Fault_Chip_position.push_back(rand()%CHIP_NUM); // Fault_Chip_position[1] // 0~9
            }
    
            if (Fault_Chip_position[0] != Fault_Chip_position[1]) break;
        }

        sort(Fault_Chip_position.begin(), Fault_Chip_position.end());

        //printf("Actual Fault Chip position: %d, %d\n",Fault_Chip_position[0],Fault_Chip_position[1]);

        switch (fault_type){
            case SE_NE: // 1bit
                error_injection_SE(Fault_Chip_position[0],Chip_array, oecc_type);
                break; 
            case DE_NE: // 2bit
                error_injection_DE(Fault_Chip_position[0],Chip_array, oecc_type);
                break;
            case CHIPKILL_NE: // chipkill
                error_injection_CHIPKILL(Fault_Chip_position[0],Chip_array, oecc_type);
                break;
            case SE_SE: // 1bit + 1bit
                error_injection_SE(Fault_Chip_position[0],Chip_array, oecc_type);
                error_injection_SE(Fault_Chip_position[1],Chip_array, oecc_type);
                break;
            case SE_DE: // 1bit + 2bit
                error_injection_SE(Fault_Chip_position[0],Chip_array, oecc_type);
                error_injection_DE(Fault_Chip_position[1],Chip_array, oecc_type);
                break;
            case SE_CHIPKILL: // 1bit + chipkill
                error_injection_SE(Fault_Chip_position[0],Chip_array, oecc_type);
                error_injection_CHIPKILL(Fault_Chip_position[1],Chip_array, oecc_type);
                break;
            case DE_DE: // 2bit + 2bit
                error_injection_DE(Fault_Chip_position[0],Chip_array, oecc_type);
                error_injection_DE(Fault_Chip_position[1],Chip_array, oecc_type);
                break;
            case DE_CHIPKILL: // 2bit + chipkill
                error_injection_DE(Fault_Chip_position[0],Chip_array, oecc_type);
                error_injection_CHIPKILL(Fault_Chip_position[1],Chip_array, oecc_type);
                break;
            case CHIPKILL_CHIPKILL: // chipkill + chipkill
                error_injection_CHIPKILL(Fault_Chip_position[0],Chip_array, oecc_type);
                error_injection_CHIPKILL(Fault_Chip_position[1],Chip_array, oecc_type);
                break;           
            default:
                break;
        }


        // 4-3. OECC
        // [1] Error를 넣은 chip에 대해서 SEC 실행
        // SEC : 136개의 1-bit error syndrome에 대응하면 correction 진행. 아니면 안함 (mis-correction을 최대한 막아보기 위함이다.)
        // DUE_position: 각 chip의 OECC에서 나온 DUE 결과. ex) 2200110000 -> 0번째, 1번째 chip에서 OECC에서 DUE 발생, 4번째, 5번째 chip에서 OECC에서 CE 발생, 나머지 chip에서는 NE
        vector<int> DUE_position;
        switch(oecc_type){
            case NO_OECC:
                break;
            case HSIAO: // 어차피 error 없으면 NE로 간주하고 correction 안하고 그냥 내보낼 것이다.
                if(recc_type==NO_RECC){
                    for(int Error_chip_pos=0; Error_chip_pos<DATA_CHIP_NUM; Error_chip_pos++) // 0~7
                        DUE_position.push_back(error_correction_oecc(Error_chip_pos, Chip_array));
                }
                else{
                    for(int Error_chip_pos=0; Error_chip_pos<CHIP_NUM; Error_chip_pos++) // 0~9
                        DUE_position.push_back(error_correction_oecc(Error_chip_pos, Chip_array));
                }
                break;
            default:
                break;
        }

        /*
        4-4. RECC

        1. 각 BL2씩 묶어서 RECC 진행
            (1) Syndrome이 모두 0이면 => return NE
            (2) Syndrome이 0이 아니고, SSC의 syndrome과 일치하면 SSC 진행 [S1/S0이 a^0~a^9중 하나] => return CE
                -> 이때, error 발생 chip 위치는 CE 일때만 return 한다. (0~9). 이때만 의미가 있기 때문!
            (3) Syndrome이 0이 아니고, SSC의 syndrome과 일치하지 않으면 => return DUE
            (4) NE/CE return 했는데, 0이 아닌 값이 남아있으면 SDC (error 감지 못한 것이기 때문! 또는 mis-correction)

        2. Cacheline 단위로 (BL16) conservative 확인
            (1) CE : 8개 RECC 결과에서 SDC,DUE가 없고 전부 NE/CE 이고, error 발생 chip 위치(0~9)가 전부 같은 경우에만 CE로 처리 (NE는 고려 X. Error가 발생하지 않았기 때문!)
            (2) DUE : 8개 RECC 결과에서 SDC가 없고, 1개라도 DUE가 있는 경우 [나머지는 NE/CE]
                ** AMDCHIPKILL만 적용되는 조건! => 또는 전부 NE/CE이지만 error 발생 chip 위치가 다른 경우
            (3) SDC : 8개의 RECC 결과에서 SDC가 1개라도 있는 경우

        3. Cacheline 2개 (BL32) conservative 확인
            (1) CE : 2개 cacheline이 전부 CE인 경우
            (2) DUE : 2개 cacheline에서 SDC가 없고 1개라도 DUE가 있는 경우
            (3) SDC : 2개 cacheline에서 1개라도 SDC가 있는 경우
        
        */
        
        // 여기는 configuration 자동화 안 시켰음 (나중에 channel size, BL등을 바꿀거면 여기는 주의해서 보기!!)
        set<int> error_chip_position; // RECC(BL2 단위)마다 error 발생한 chip 위치 저장 (CE인 경우에만 저장)
        int result_type_recc; // NE, CE, DUE, SDC 저장
        int first_cacheline_conservative,second_cacheline_conservative;
        int final_result, final_result_1=CE,final_result_2=CE; // 각각 2개 cachline 고려한 최종 결과, 첫번째 cacheline, 두번째 cacheline 검사 결과
        int isConservative=0;
        int first_error_chip_position=-1, second_error_chip_position=-1;
        int SMART_ECC_MODE=0;
        switch(recc_type){ 
            case AMDCHIPKILL: // GF(2^8) 기준
                // 첫번째 cacheline
                for(int BL=0; BL<16; BL+=2){ // BL<16
                    error_correction_recc_AMDCHIPKILL(BL, &result_type_recc, error_chip_position, Chip_array);

                    // SDC 검사 (1이 남아있으면 SDC)
                    if(result_type_recc==CE || result_type_recc==NE){
                        int error_check=SDC_check(BL, Chip_array, oecc_type, recc_type);
                        if(error_check){
                            result_type_recc=SDC;
                        }
                    }
                    // DUE 검사 1
                    if(result_type_recc==DUE || final_result_1==DUE)
                        final_result_1=DUE;
                    else{ // 둘 중 우선순위가 큰 값 (SDC > CE > NE), 이전에 DUE가 나온 적이 없는 경우에만 들어갈 수 있다.
                        final_result_1 = (final_result_1>result_type_recc) ? final_result_1 : result_type_recc;
                    }

                    // DUE 검사 2 (Conservative mode 켜져있을 때만 진행!!)
                    // 다른 chip에서 Correction 진행한 적 있으면 true로 켜진다. => DUE 위함
                    if(CONSERVATIVE_MODE)
                        isConservative = (error_chip_position.size()>1) ? 1 : isConservative;
                }

                if(final_result_1==NE || final_result_1==CE){
                    final_result_1 = (isConservative) ? DUE : CE;
                }

                // 두번째 cacheline
                error_chip_position.clear();
                isConservative=0;
                for(int BL=16; BL<32; BL+=2){ // BL : 16~31
                    error_correction_recc_AMDCHIPKILL(BL, &result_type_recc, error_chip_position, Chip_array);

                    // SDC 검사 (1이 남아있으면 SDC)
                    if(result_type_recc==CE || result_type_recc==NE){
                        int error_check=SDC_check(BL, Chip_array, oecc_type, recc_type);
                        if(error_check){
                            result_type_recc=SDC;
                        }
                    }
                    // DUE 검사 1
                    if(result_type_recc==DUE || final_result_2==DUE)
                        final_result_2=DUE;
                    else{ // 둘 중 우선순위가 큰 값 (SDC > CE > NE), 이전에 DUE가 나온 적이 없는 경우에만 들어갈 수 있다.
                        final_result_2 = (final_result_2>result_type_recc) ? final_result_2 : result_type_recc;
                    }

                    // DUE 검사 2 (Conservative mode 켜져있을 때만 진행!!)
                    // 다른 chip에서 Correction 진행한 적 있으면 true로 켜진다. => DUE 위함
                    if(CONSERVATIVE_MODE)
                        isConservative = (error_chip_position.size()>1) ? 1 : isConservative;
                }

                if(final_result_2==NE || final_result_2==CE){
                    final_result_2 = (isConservative) ? DUE : CE;
                }


                // 2개 cacheline 비교해서 최종결과 update
                // SDC : 2개 cacheline 중에서 1개라도 SDC가 있으면 전체는 SDC
                // DUE : 2개 cacheline 중에서 SDC가 없고, 1개라도 DUE가 있으면 전체는 DUE
                // CE : 그 외 경우 (둘 다 CE)
                final_result = (final_result_1 > final_result_2) ? final_result_1 : final_result_2;
                break;
            case SMARTECC: // GF(2^8) 기준
                // SMART ECC MODE SETTING
                // ERROR/ERASURE decoding 중에서 어느 것으로 할지 DUE 개수로 결정
                fflush(fp3);
                for (vector<int>::size_type error_index=0; error_index<DUE_position.size(); error_index++){
                    if(DUE_position[error_index]==DUE && first_error_chip_position==-1)
                        first_error_chip_position=error_index;
                    else if (DUE_position[error_index]==DUE && first_error_chip_position!=-1)
                        second_error_chip_position=error_index;
                }
                if(count(DUE_position.begin(), DUE_position.end(), DUE) < 2) // DUE 개수 2개 미만
                    SMART_ECC_MODE=0;
                else if(count(DUE_position.begin(), DUE_position.end(), DUE) == 2) // DUE 개수 2개
                    SMART_ECC_MODE=1;
                else // DUE 개수 3개 이상
                    SMART_ECC_MODE=2;

                //fprintf(fp3,"runtime %d\n",runtime);
                //fprintf(fp3,"SMART ECC MODE: %d\n",SMART_ECC_MODE);

                //SMART_ECC_MODE=1;
                //first_error_chip_position=Fault_Chip_position[0];
                //second_error_chip_position=Fault_Chip_position[1];

                // 첫번째 cacheline
                //printf("Smart ECC first cacheline!\n");
                for(int BL=0; BL<16; BL+=2){ // BL<16
                    if(SMART_ECC_MODE==0)
                        error_correction_recc_SMARTECC(BL, &result_type_recc, error_chip_position, Chip_array);
                    else if(SMART_ECC_MODE==1)
                        erasure_correction_recc_SMARTECC(BL, &result_type_recc, error_chip_position, Chip_array, first_error_chip_position, second_error_chip_position);
                    else // DUE 개수 3개 이상
                        result_type_recc=DUE;

                    // SDC 검사 (1이 남아있으면 SDC)
                    if(result_type_recc==CE || result_type_recc==NE){
                        int error_check=SDC_check(BL, Chip_array, oecc_type, recc_type);
                        if(error_check){
                            result_type_recc=SDC;
                        }
                    }
                    // DUE 검사 1
                    if(result_type_recc==DUE || final_result_1==DUE)
                        final_result_1=DUE;
                    else{ // 둘 중 우선순위가 큰 값 (SDC > CE > NE), 이전에 DUE가 나온 적이 없는 경우에만 들어갈 수 있다.
                        final_result_1 = (final_result_1>result_type_recc) ? final_result_1 : result_type_recc;
                    }

                    // DUE가 1개 이하일때는 AMD Chipkill과 같이 conservative mode 적용
                    // 다른 chip에서 Correction 진행한 적 있으면 true로 켜진다. => DUE 위함
                    //if(CONSERVATIVE_MODE && SMART_ECC_MODE==0)
                    //    isConservative = (error_chip_position.size()>1) ? 1 : isConservative;
                }

                if(final_result_1==NE || final_result_1==CE){
                    final_result_1 = (isConservative) ? DUE : CE;
                }

                getchar();

                // 두번째 cacheline
                //printf("Smart ECC second cacheline!\n");
                error_chip_position.clear();
                isConservative=0;
                for(int BL=16; BL<32; BL+=2){ // BL : 16~31
                    if(SMART_ECC_MODE==0)
                        error_correction_recc_SMARTECC(BL, &result_type_recc, error_chip_position, Chip_array);
                    else if(SMART_ECC_MODE==1)
                        erasure_correction_recc_SMARTECC(BL, &result_type_recc, error_chip_position, Chip_array, first_error_chip_position, second_error_chip_position);
                    else // DUE 개수 3개 이상
                        result_type_recc=DUE;

                    // SDC 검사 (1이 남아있으면 SDC)
                    if(result_type_recc==CE || result_type_recc==NE){
                        int error_check=SDC_check(BL, Chip_array, oecc_type, recc_type);
                        if(error_check){
                            result_type_recc=SDC;
                        }
                    }
                    // DUE 검사 1
                    if(result_type_recc==DUE || final_result_2==DUE)
                        final_result_2=DUE;
                    else{ // 둘 중 우선순위가 큰 값 (SDC > CE > NE), 이전에 DUE가 나온 적이 없는 경우에만 들어갈 수 있다.
                        final_result_2 = (final_result_2>result_type_recc) ? final_result_2 : result_type_recc;
                    }

                    // DUE가 1개 이하일때는 AMD Chipkill과 같이 conservative mode 적용
                    // 다른 chip에서 Correction 진행한 적 있으면 true로 켜진다. => DUE 위함
                    //if(CONSERVATIVE_MODE && SMART_ECC_MODE==0)
                    //    isConservative = (error_chip_position.size()>1) ? 1 : isConservative;
                }

                if(final_result_2==NE || final_result_2==CE){
                    final_result_2 = (isConservative) ? DUE : CE;
                }


                // 2개 cacheline 비교해서 최종결과 update
                // SDC : 2개 cacheline 중에서 1개라도 SDC가 있으면 전체는 SDC
                // DUE : 2개 cacheline 중에서 SDC가 없고, 1개라도 DUE가 있으면 전체는 DUE
                // CE : 그 외 경우 (둘 다 CE)
                final_result = (final_result_1 > final_result_2) ? final_result_1 : final_result_2;
                break;
            case NO_RECC:
                int error_check;
                error_check = Yield_check(Chip_array, oecc_type, recc_type);
                final_result = (error_check>0) ? SDC : CE;
                break;
            default:
                break;
        }

        // 4-5. CE/DUE/SDC 체크
        // 최종 update (2개 cacheline 전부 고려)
        // CE, DUE, SDC 개수 세기
        //printf("final_result: %d\n",final_result);
        CE_cnt   += (final_result==CE)  ? 1 : 0;
        DUE_cnt  += (final_result==DUE) ? 1 : 0;
        SDC_cnt  += (final_result==SDC) ? 1 : 0;
            
    }
    // for문 끝!!

    // 최종 update
    fprintf(fp3,"\n===============\n");
    fprintf(fp3,"Runtime : %d\n",RUN_NUM);
    fprintf(fp3,"CE : %d\n",CE_cnt);
    fprintf(fp3,"DUE : %d\n",DUE_cnt);
    fprintf(fp3,"SDC : %d\n",SDC_cnt);
    fprintf(fp3,"\n===============\n");
    fflush(fp3);

    // 최종 update (소숫점 표현)
    fprintf(fp3,"\n===============\n");
    fprintf(fp3,"Runtime : %d\n",RUN_NUM);
    fprintf(fp3,"CE : %.11f\n",(double)CE_cnt/(double)RUN_NUM);
    fprintf(fp3,"DUE : %.11f\n",(double)DUE_cnt/(double)RUN_NUM);
    fprintf(fp3,"SDC : %.11f\n",(double)SDC_cnt/(double)RUN_NUM);
    fprintf(fp3,"\n===============\n");
    fflush(fp3);

    fclose(fp3);


    return 0;
}
