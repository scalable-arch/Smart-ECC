#!/bin/csh -f

cd /home/xyz123479/TYPEMOON/SmartECC/Smart-ECC/Dongwhee/RTL/Rank_Level_ECC/10_8_RS_Erasure_Decoding/Decoder/sim/output

#This ENV is used to avoid overriding current script in next vcselab run 
setenv SNPS_VCSELAB_SCRIPT_NO_OVERRIDE  1

/usr/synopsys/vcs/R-2020.12-SP1-1/linux64/bin/vcselab $* \
    -o \
    simv \
    -nobanner \

cd -

