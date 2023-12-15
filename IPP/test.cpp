#include <mcl/bls12_381.hpp>
#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <malloc.h>
#include <mcl/ntt.hpp>
#include <chrono>
#include <fstream>
#include <iomanip> 
#include "IPP.hpp"
#include "polyonfr.hpp"


using namespace mcl;
using namespace mcl::bn;
using namespace std;
using namespace chrono;


void IPP_test(const string& file_name) {
    vector<G2> gg;
    vector<G1> ww;
    ofstream fout(file_name, ios::app);

    fout << "input size, Prove time (microseconds), Verification time (microseconds), naive pairing(microsecond), Proof Size(KB)" << endl;

    for(int size = 128; size <= 16384; size *= 2) {
        microseconds elapsed;

        int adjusted_size = nextPowOf2(size);
        fout << adjusted_size << ", ";

        gg.resize(size);
        ww.resize(size);

        Fr r;
        r.setByCSPRNG();

        for(uint32_t i = 0; i < ww.size(); i++) {
            hashAndMapToG1(ww[i], to_string(i));
            hashAndMapToG2(gg[i], to_string(i));
            gg[i]=gg[i]*r;
            r=r*r;
        }

        GT P = MultiPairing(ww, gg);

        auto start = steady_clock::now();
        IPPproof ipp_proof(gg, P);
        ipp_proof.Prove(gg, P, ww);
        auto end = steady_clock::now();
        elapsed = duration_cast<microseconds>(end - start);
        fout << elapsed.count() << ", ";

        auto start_2 = steady_clock::now();
        bool flag = IPPverify(ipp_proof);
        auto end_2 = steady_clock::now();
        elapsed = duration_cast<microseconds>(end_2 - start_2);
        fout << elapsed.count() << ", ";
        cout<<flag<<endl;

        auto start_3 = steady_clock::now();
        for (int i = 0; i < size; ++i) {
        GT result; 
        pairing(result,ww[i], gg[i]);
        }
        auto end_3 = steady_clock::now();
        elapsed = duration_cast<microseconds>(end_3 - start_3);
        fout << elapsed.count() << ", ";

        float N = ipp_proof.L.size()*sizeof(ipp_proof.L[0])+ipp_proof.R.size()*sizeof(ipp_proof.R[0])
        +sizeof(ipp_proof.w);

        fout << N/1024 << ", ";

        fout << endl;
    }
}

void zkIPP_test(const std::string& file_name){

    vector<G2> gg;
    vector<G1> ww;

    std::ofstream fout1(file_name, std::ios::app);

    fout1<<"input size, "<<"Prove time, "<<"verification time, "<<endl;

    //int values[] = {127, 255, 511, 1023, 2047, 4095, 8191, 16383};

    for(int i=128; i<=16384; i=i*2){

        
        i=i-1;
        microseconds elaps;

        i=nextPowOf2(i);
        fout1<< i << ", ";

        gg.resize(i);
        ww.resize(i);

        for(uint32_t i=0;i<ww.size();i++){

            hashAndMapToG1(ww[i],to_string(i));
            hashAndMapToG2(gg[i],to_string(i));

        }

        GT P =MultiPairing(ww,gg);

        auto start_1 = chrono::steady_clock::now();
        IPPproof ipp_pi = zkIPPprove(gg,P,ww);

        auto end_1 = chrono::steady_clock::now();
        elaps = duration_cast<microseconds>(end_1-start_1);
        fout1<< elaps.count()<< ", ";
        

        auto start_2 = chrono::steady_clock::now();

        bool flag = zkIPPverify(ipp_pi);

        auto end_2 = chrono::steady_clock::now();

        elaps = duration_cast<microseconds>(end_2-start_2);
        fout1<< elaps.count()<< ", ";

        fout1<<endl;
        


    }


}

int main(){

    initPairing(mcl::BLS12_381);

    IPP_test("ipp_performance.csv");



    return 0;
}
