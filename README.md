
## About SmartECC

This work is for ITTP PIM R&D program.


## Building and Running

1. Install requirements for `gcc`, `g++`, `make`
```bash
sudo apt install build-essential
```
                 

2. make and running 
```bash
# make binary file to run this code
make

# run this code 
# ./test.out -input (codeword) -error_length (length) -error_pos (list of position index) 
./test.out -input 1 -error_length 1 -error_pos 130
./test.out -input 1 -error_length 2 -error_pos 1 2

```

## Documentation

Please visit to https://quark-lupin-c50.notion.site/SmartECC-Architecture-9310b61452514e469ecf84551de7adca
