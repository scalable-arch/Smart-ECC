#include <stdio.h>
#include <string.h>

#include "Tester.hh"
#include "utils/common.hh"


void parseArgs(int& argc, char **argv, Param& params){

    /* parse the command line arguments */
   int arg_index = 1;
    while (arg_index != argc)
    {
        if (!strcmp(argv[arg_index], "-input")){
            params.data_i = Data(atoi(argv[arg_index + 1]));
            arg_index += 2;
            continue;
        }

        if (!strcmp(argv[arg_index], "-error_length")){
            params.error_length =  atoi(argv[arg_index + 1]);
            params.error_loc = new int[params.error_length];
            arg_index += 2;
            continue;
        }

        if (!strcmp(argv[arg_index], "-error_pos")){
            for(int i=0; i<params.error_length; i++){
                params.error_loc[i] = atoi(argv[arg_index+1]);
                arg_index +=1;                
            }
            arg_index +=1;
            continue;
        }

        printf("error:  unrecognized flag %s\n", argv[arg_index]);
        assert(0);
    }
}


int main(int argc, char **argv){
    
    /* paramseter */
    Param params;
    parseArgs(argc, argv, params);

    int error_length; 
    int* errorLocation;


    // Test functions
    Tester* tester = new Tester(&params);
    tester->run();    

}




