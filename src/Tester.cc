#include "Tester.hh"

//------------------------------------------------------------------------------
Tester::Tester(Param* _param) 
: param(_param), hsiao(new Hsiao(72, 8)), cnn(new CNN()), mld(new MLD()){

}


//------------------------------------------------------------------------------
void Tester::run(){

    //Step1. Encode and Gen the packet
    Codeword encode_o = hsiao->encode(param->data_i);
    Packet* packet = new Packet(RAND_MAX*((ADDR)rand()) + rand(), encode_o);


    //Step2. Error Injection
    packet->injectError(param->error_length, param->error_loc);
    std::cout << std::setw(20) <<"After Err Injected: " << (*packet->getCodeword()) << std::endl;


    //Step3. Generate Candidates
    Syndrome syn = hsiao->genSydrome((*packet->getCodeword()));
    Candidate candidates = hsiao->getCandidate(syn);


    //Step4. Get BER per cell
    double* likelihood = cnn->forward(packet);
    Codeword best_candidate = mld->rankCandidate(packet, candidates, likelihood);


    // Step5. Bit Inversion
    packet->invBit(best_candidate);
    std::cout << std::setw(20) <<"Best Candidate: " << best_candidate << std::endl;
    std::cout << std::setw(20) <<"After Correction: " << (*packet->getCodeword()) << std::endl;


    // Step6. Feedback
    Codeword label = getLabel();
    cnn->backward(label, likelihood);
}



//------------------------------------------------------------------------------
Codeword Tester::getLabel(){
    Codeword label_mask = Codeword(0);

    for(int i=0; i<param->error_length; i++){
        int loc = param->error_loc[i];
        label_mask[loc] = 1;
    }  

    return label_mask;
}

