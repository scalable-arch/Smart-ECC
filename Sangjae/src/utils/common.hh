#ifndef __COMMON_HH__
#define __COMMON_HH__

#include "config.hh"


typedef unsigned long long ADDR;
typedef std::bitset<CODEWORD_LENGTH> Codeword;
typedef std::bitset<DATA_LENGTH> Data;
typedef std::bitset<REDUNDANCY_LENGTH> Syndrome;
typedef std::vector<std::pair<int, int>> Candidate;

struct Param
{
    /* Test Configuration */
    Data data_i = 0;
    int error_length = 0;
    int* error_loc = nullptr;
};

#endif