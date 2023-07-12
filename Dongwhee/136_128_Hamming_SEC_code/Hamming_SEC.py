import numpy as np
import random

def main():
    H_Matrix=np.loadtxt("H_Matrix.txt", dtype="int")
    redundancy_len=len(H_Matrix) # 8
    codeword_length=len(H_Matrix[0]) # 136
    data_length=codeword_length-redundancy_len # 128

    cnt=0
    iteration_num=10000
    CE_cnt=0
    DUE_cnt=0
    SDC_cnt=0
    while cnt<iteration_num:
        NE_case=0
        CE_case=0
        DUE_case=0
        SDC_case=0
        # codeword initialization (no-error)
        codeword=np.zeros(codeword_length, dtype=int)

        # error injection (multi-bit error)
        # error position : 0~135 (50% probability bit-flip)
        for index in range(codeword_length):
            bit_flip=random.randrange(0, 2)
            codeword[index]^=bit_flip

        #print("Received codeword: ",codeword)

        # Calculate Syndrome
        codeword_transpose = codeword.transpose()
        Syndrome=(np.matmul(H_Matrix, codeword_transpose))%2

        # NE (SDC) case
        if np.all(Syndrome==0):
            NE_case=1
        else: # CE (SDC) 'or' DUE case
            for error_index in range(codeword_length):
                if np.array_equal(H_Matrix[:,error_index],Syndrome):
                    codeword[error_index]^=1
                    CE_case=1
                    break
            if(CE_case==0):
                DUE_case=1

        # SDC check
        if CE_case==1 or NE_case==1:
            if not np.all(Syndrome==0): # error remain
                NE_case=0
                CE_case=0
                SDC_case=1

        # count CE, DUE, SDC
        if NE_case or CE_case:
            CE_cnt+=1
        elif DUE_case:
            DUE_cnt+=1
        else:
            SDC_cnt+=1

        # Continue to next iteration
        cnt+=1
    
    print("CE_cnt : {0}".format(CE_cnt))
    print("DUE_cnt : {0}".format(DUE_cnt))
    print("SDC_cnt : {0}".format(SDC_cnt))

if __name__ == "__main__":
    main()
