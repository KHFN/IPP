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
