# RS Erasure Decoding against Error Scenario
# Read '[2023-08-17] 정리자료_RS erasure decoding_Systematic format.pptx'

# Author

**Dongwhee Kim** 
- Email: xyz12347976@gmail.com
- Google Scholar: https://scholar.google.com/citations?user=8xzqA8YAAAAJ&hl=ko&oi=ao

# Objectives
- Implement **On-Die ECC (OD-ECC) [1-2]** and **Rank-Level ECC (RL-ECC)** of DDR5 ECC-DIMM
- Implement RS Erasure Decoding using Alert_n pin (DUE information)

# Overview
![An Overview of the RS Erasure Decoding](https://github.com/scalable-arch/Smart-ECC/blob/main/Dongwhee/Erasure_Decoding_Simulation/Smart%20ECC_RS%20Erasure%20Decoding.png)

# Code flows (Fault_sim.cpp)
- 1. Reading OD-ECC H-Matrix.txt
- 2. Setting output function name: output.S file.
- 3. **(Start loop)** DDR5 ECC-DIMM setup
- 4. Initialize all data in 10 chips to 0: Each chip has 136 bits of data + redundancy.
- 5. Error injection: Errors occur based on the error scenarios
- 6. Apply OD-ECC: Implementation
>> Apply the Hamming SEC code of (136, 128) to each chip.

>> After running OD-ECC, the redundancy of OD-ECC does not come out of the chip (128bit data).

>> DUE information is extracted from each chip
- 7. Apply RL-ECC (Using DUE information of each chip)
>> Run (80, 64) RL-ECC by bundling two beats.

>> Please feel free to use any ECC code.

>> 16 Burst Length (BL) creates one memory transfer block (64B cacheline + 16B redundancy).

>> In DDR5 x4 DRAM, because of internal prefetching, only 64bit of data from each chip's 128bit data is actually transferred to the cache.

>> For this, create two memory transfer blocks for 128-bit data and compare them.
- 8. Report CE/DUE/SDC results.
- 9. **(End loop)** Derive final results.

# DIMM configuration (per-sub channel)
- DDR5 ECC-DIMM
- Num of rank: 1
- Beat length: 40 bit
- Burst length: 16
- Num of data chips: 8
- Num of parity chips: 2
- Num of DQ: 4 (x4 chip)

# ECC configuration
- OD-ECC: (136, 128) Hamming SEC code **[1]**
- RL-ECC: (80,64) RS code Erasure Decoding
>> Restrained mode **[3]**

# Error pattern configuration
- SE(SBE): per-chip Single Bit Error
- DE(DBE): per-chip Double Bit Error
- CHIPKILL(SCE): Single Chip Error (All Random)

# Error Scenario configuration
- SE(SBE): Among 10 chips, there's a single bit error (SE[Single Bit Error]) occurring in just one chip, with the remaining 9 chips having no errors
- DE(DBE): Among 10 chips, there's a double bit error (DE[Double Bit Error]) occurring in just one chip, with the remaining 9 chips having no errors
- CHIPKILL(SCE): Among 10 chips, there's a random error (SCE [Single Chip Error]) occurring in just one chip, with the remaining 9 chips having no errors. Errors can occur up to a maximum of 136 bits
- SE(SBE)+SE(SBE): Among 10 chips, there's a single bit error (SE[Single Bit Error]) occurring in each of two chips, with the remaining 8 chips having no errors
- ...

# Getting Started
- $ make clean
- $ make
- $ python run.py

# Answer (.S files)
- Errors can be injected randomly, thus there may be slight discrepancies each time it is executed

# Additional Information
- NE: no error
- CE: detected and corrected error
- DUE: detected but uncorrected error
- SDC: Silent Data Corruption

# References
- **[1]** Hamming, Richard W. "Error detecting and error correcting codes." The Bell system technical journal 29.2 (1950): 147-160.
- **[2]** M. JEDEC. 2022. DDR5 SDRAM standard, JESD79-5B𝑣 1.20.
- **[3]** Kim, Dongwhee, et al. "Unity ECC: Unified Memory Protection Against Bit and Chip Errors." Proceedings of the International Conference for High Performance Computing, Networking, Storage and Analysis. 2023.

