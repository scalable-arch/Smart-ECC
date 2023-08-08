from multiprocessing import Pool
import sys
import os
import time

oecc = [1] # 0 : no-ecc, 1 : SEC (Hsiao)
fault = [0,1,2,3,4,5,6,7,8] # SE_NE=0, DE_NE=1, CHIPKILL_NE=2, SE_SE=3, SE_DE=4, SE_CHIPKILL=5, DE_DE=6, DE_CHIPKILL=7, CHIPKILL_CHIPKILL=8
recc = [0,1] # 0 : AMD Chipkill, 1 : Smart CXL ECC, 2: No-Rank level ECC

for oecc_param in oecc:
    for fault_param in fault:
        for recc_param in recc:
            os.system("./Fault_sim_start {0:d} {1:d} {2:d} &".format(oecc_param, fault_param, recc_param))

# SE (SBE): per-chip Single Bit Error
# DE (DBE): per-chip Double Bit Error
# CHIPKILL (SCE): Single Chip Error 