
#include "MLD.hh"

void MLD::getFloatAND(double* p, Codeword data, double* and_p){

    for(int i=0; i<data.size(); i++){
        and_p[i] = p[i];
        if(data[i] != 1)
            and_p[i] = 0;
    }
}



//------------------------------------------------------------------------------
Codeword MLD::rankCandidate(Packet* packet_i, Candidate candidate_i, double* bep_i){
    
    Codeword dram_rd_i = (*packet_i->getCodeword());
    double* total_err = new double[candidate_i.size()];
    int k=0;
    // Coalescing (implemented by SIMD)
    for(auto it=candidate_i.begin(); it!=candidate_i.end(); it++, k++){

        double err_prob=0.;

        err_prob += bep_i[(*it).first];
        err_prob += bep_i[(*it).second];
        
        total_err[k] = err_prob;
    }

    // printf("------- Candidate List ------- \n");
    // int m=0;
    // for(auto it=candidate_i.begin(); it!=candidate_i.end(); m++, it++){
    //     printf("\t pair1:%d \tpair2:%d \tErrLikelihood: %f \n", 
    //             (*it).first, (*it).second, total_err[m]);
    // }


    // Ranking (find the maximum)
    double err_max = -1;
    int d=0;
    for(int i=0; i<candidate_i.size(); i++){
        if(err_max < total_err[i]){
            d = i;
            err_max = total_err[i];
        }
    }


    // Gen the Mask
    Codeword err_mask = Codeword(0);
    err_mask[candidate_i[d].first] = 1;
    err_mask[candidate_i[d].second] = 1;

    return err_mask;
}


