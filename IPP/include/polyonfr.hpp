#ifndef POLYONFR_H__
#define POLYONFR_H__

#include <mcl/bls12_381.hpp>
#include <iostream>
#include <cmath>


using namespace mcl;
using namespace mcl::bn;
using namespace std;

bool IsPowerOfTwo(uint64_t v);

uint32_t Max(uint32_t x, uint32_t y);

uint32_t Min(uint32_t x, uint32_t y);


typedef std::vector<Fr> FrVec;

uint64_t nextPowOf2(const uint64_t v);

bool IsPolyZero(FrVec a);

FrVec PolyCondense(FrVec a);


bool IsPolyEqual(FrVec a, FrVec b);


FrVec PolyAdd(FrVec a, FrVec b);

//a-b 
FrVec PolySub(FrVec a, FrVec b);


FrVec PolyMul(const FrVec a, const FrVec b);


//A(x)/B(x)
FrVec PolyLongDiv(FrVec A, FrVec B);

Fr PolyEvaluate(const FrVec &a, const Fr &x);


tuple<FrVec,FrVec> PolyDiv(FrVec A, FrVec B);


tuple<FrVec,FrVec> PolyDiv_na(FrVec A, FrVec B);

FrVec PolyDifferentiate(FrVec A);

#endif