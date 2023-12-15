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

| Input Size | Prove Time (s) | Verification Time (s) | Naive Pairing (s) |
|------------|----------------|-----------------------|-------------------|
| 128        | 0.32           | 0.05                  | 0.34              |
| 256        | 0.60           | 0.07                  | 0.68              |
| 512        | 1.19           | 0.12                  | 1.36              |
| 1024       | 2.31           | 0.21                  | 2.71              |
| 2048       | 4.60           | 0.38                  | 5.43              |
| 4096       | 9.08           | 0.67                  | 10.83             |
| 8192       | 18.06          | 1.28                  | 21.70             |
| 16384      | 35.97          | 2.58                  | 43.35             |


| Input Size | Prove Time (µs) | Verification Time (µs) | Naive Pairing (µs) | Proof Size (KB) |
|------------|------------------|------------------------|--------------------|-----------------|
| 128        | 116429           | 15403                  | 138691             | 8.01562         |
| 256        | 219010           | 30029                  | 236493             | 9.14062         |
| 512        | 397441           | 39646                  | 452954             | 10.2656         |
| 1024       | 762610           | 78046                  | 1042721            | 11.3906         |
| 2048       | 1576852          | 134696                 | 1921920            | 12.5156         |
| 4096       | 3189819          | 262555                 | 3755546            | 13.6406         |
| 8192       | 6696170          | 603221                 | 8156698            | 14.7656         |
| 16384      | 13478960         | 960940                 | 14614482           | 15.8906         |
