#ifndef __TESTER_HH__
#define __TESTER_HH__

#include <stdio.h>
#include <string>
#include <map>

#include "mem_local/hsiao.hh"
#include "mem_local/CNN.hh"
#include "mem_local/MLD.hh"
#include "mem_global/Faultmap.hh"
#include "utils/common.hh"

class Tester{
public:
    Tester(Param* _param);

public:
    void run();

protected:
    Codeword getLabel();

private:
    Param* param;
    Hsiao* hsiao;
    MLD* mld;
    CNN* cnn;
};


#endif