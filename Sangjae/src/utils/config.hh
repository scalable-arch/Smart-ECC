#ifndef __CONFIG_HH__
#define __CONFIG_HH__

#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <bitset>
#include <iomanip>
#include <assert.h>
#include <time.h>
#include <vector>
#include <bitset>
#include <set>

#define DATA_LENGTH 64
#define REDUNDANCY_LENGTH 8
#define CODEWORD_LENGTH 72

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c ---\n"
#define BYTE_TO_BINARY(byte)\
  (byte & 0x80 ? '1' : '0'),\
  (byte & 0x40 ? '1' : '0'),\
  (byte & 0x20 ? '1' : '0'),\
  (byte & 0x10 ? '1' : '0'),\
  (byte & 0x08 ? '1' : '0'),\
  (byte & 0x04 ? '1' : '0'),\
  (byte & 0x02 ? '1' : '0'),\
  (byte & 0x01 ? '1' : '0') 



/* Target: DDR4 x8 8Gb Device */
#define CODEWORD_LENGTH     72
#define CODEWORD_MASK       0x1FFFFFFFFULL      // Codeword Mask (33bit)
#define COLUM_MASK          0x000001FC0ULL      // Column Mask (13bit)
#define ROW_MASK            0x01FFFE000ULL      // Row Mask (16)
#define BANK_MASK           0x1E0000000ULL      // 

#define BANK_OFFSET   29
#define ROW_OFFSET  13      
#define COL_OFFSET  6                           // 2^6 = 64

#define ROWBIT  16  // 64k rows
#define COLBIT  13  // 64k rows


/* CNN */
#define LEARNING_RATE 0.001
#define NUM_LAYER 1
#define WIDTH 3
#define HEIGHT 2


#endif