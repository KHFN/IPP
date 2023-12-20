This project makes use of the [mcl](https://github.com/herumi/mcl) library, 
which is a portable and fast pairing-based cryptography library developed by MITSUNARI Shigeo.

## How to Build and Run Tests
Use x86-64 Linux.
Before running the tests, you need to build and set up the necessary libraries. Follow these steps:

1. **Clone the Project**:
   ```bash
   git clone https://github.com/KHFN/IPP.git
   cd IPP
   ```

2. **Clone and Build the mcl Library**:
   ```bash
   git clone https://github.com/herumi/mcl
   cd mcl
   make -j4
   mkdir build
   cd build
   cmake ..
   make
   cd ../..
   ```

3. **Compile and Run the Test**:
   ```bash
   cd IPP
   g++ -o test test.cpp -L./lib -lIPP -L../mcl/lib -lmcl -I../mcl/include -I./include -L./lib -lpoly
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../mcl/lib
   ./test
   ```

After completing these steps, you can run the project's test by executing `./test`.

## Test Results example

The tests were conducted in the following environment:

- **OS**: Ubuntu 18.04.6 LTS
- **CPU**: Intel(R) Xeon(R) CPU E5-2620 v3 @ 2.40GHz
- **Memory**: 64GB RAM

The test results are as follows:

| Input Dim | Proving Time |      | Verification Time |      | Pairing Time |      | Proof Size |
|-----------|--------------|------|-------------------|------|--------------|------|------------|
|           | Mean         | Std  | Mean              | Std  | Mean         | Std  | N/A        |
| 128       | 0.32         | 0.00079 | 0.048           | 0.00013 | 0.034      | 0.0004 | 8.02KB    |
| 256       | 0.6          | 0.0013  | 0.074           | 0.00016 | 0.067      | 0.00028 | 9.14KB    |
| 512       | 1.17         | 0.003   | 0.12            | 0.00018 | 1.35       | 0.00057 | 10.27KB   |
| 1024      | 2.29         | 0.0059  | 0.21            | 0.00038 | 2.7        | 0.0014  | 11.39KB   |
| 2048      | 4.53         | 0.012   | 0.38            | 0.00028 | 5.4        | 0.002   | 12.52KB   |
| 4096      | 9.02         | 0.02    | 0.66            | 0.00066 | 10.78      | 0.0056  | 13.64KB   |
| 8192      | 18           | 0.045   | 1.27            | 0.0022  | 21.6       | 0.011   | 14.77KB   |
| 16384     | 35.93        | 0.085   | 2.56            | 0.0029  | 43.21      | 0.019   | 15.89KB   |

100회의 반복 후 평균/ Annealing 5회


