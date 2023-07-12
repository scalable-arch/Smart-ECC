#include "CNN.hh"


//------------------------------------------------------------------------------
CNN::CNN():weight(new double[WIDTH*HEIGHT]), fm(new FaultMap()){
    // TODO: implement Xavier-Initialization
    // random initialization
    for(int i=0; i<WIDTH*HEIGHT; i++){
        weight[i] = double(rand())/RAND_MAX;
    }

    // DPRINTF
    printf("Before Weight Update \n");
    for(int r=0; r<HEIGHT; r++){
        for(int c=0; c<WIDTH; c++){
            printf("\t%f ", weight[c]);
        }
        printf("\n");
    }

}



//------------------------------------------------------------------------------
double* CNN::forward(Packet* packet_i){

    double* likelihood_o = new double[CODEWORD_LENGTH-1];
    
    fault_map = fm->getFaultMap(packet_i);
    cw = packet_i->getCodeword();

    // Start Convolution
    // TODO: Accelerating
    for(int start=0; start<CODEWORD_LENGTH-WIDTH; start++){
        double sum=0;
        for(int col=start; col<start+WIDTH; col++){
            sum += fault_map[col]*weight[col-start];
            sum += (cw[col]==1)? weight[WIDTH+col-start] : 0;
        }
        likelihood_o[start] = sum;
    }

    return likelihood_o;
}




//------------------------------------------------------------------------------
void CNN::backward(Codeword label_i, double* likelihood_i){
    
    // Example
    // out:   1e-3  0  1e-5  0.1 ...
    // label:   0   0    0    0  ...
    //
    

    // L2_loss = (oi-yi)^2
    // deriv(L2_loss) = 2(oi-yi)
    double* derivatives = new double[CODEWORD_LENGTH];
    for(int i=0; i<CODEWORD_LENGTH; i++){
        derivatives[i] = 2*(likelihood_i[i]-label_i[i]);
    }


    // Weight update
    // new_weight = weight - lr*input*derivatives[i]
    for(int start=0; start<CODEWORD_LENGTH-1; start++){
        for(int col=start; col<start+WIDTH; col++){
            weight[col-start] -= LEARNING_RATE*(fault_map[col]*derivatives[col]);
            weight[WIDTH+col-start] -= (cw[col]==1)? LEARNING_RATE*derivatives[col] : 0;
        }
    }


    // DPRINTF
    // printf("After Weight Update \n");
    // for(int r=0; r<HEIGHT; r++){
    //     for(int c=0; c<WIDTH; c++){
    //         printf("\t%f ", weight[c]);
    //     }
    //     printf("\n");
    // }
}

