#ifndef IPP_H__
#define IPP_H__


#include <mcl/bls12_381.hpp>
#include <iostream>
#include <vector>
#include "polyonfr.hpp"

using namespace mcl;
using namespace mcl::bn;
using namespace std;

template <typename T>
void putG(vector<T> a);


/*
a in GG1^n, b in GG2
return \prod_{i=0}^{n-1} e(a[i],b[i]);
*/
GT MultiPairing(vector<G1> a, vector<G2> b);

struct IPPproof{

        vector<G2> gg;
        GT P;
        GT Q;
        vector<GT> L;
        vector<GT> R;
        vector<Fr> x;
        G1 w;
        G2 g;

        IPPproof(vector<G2> gg_0, GT P){
                this->gg=gg_0;
                this->P=P;
                this->L.clear();
                this->R.clear();
                this->x.clear();
        }

        void Prove(vector<G2> gg, GT P, vector<G1> ww){
            if((IsPowerOfTwo(ww.size())==false) || (IsPowerOfTwo(gg.size())==false)){
                throw runtime_error("gg and ww need to be power of two");
            }

            if(ww.size()==1){
                this->w = ww[0];
                this->g = gg[0];
                
            }else{

                vector<G2> gg_L(gg.begin(), gg.begin()+gg.size()/2);
                vector<G2> gg_R(gg.begin()+gg.size()/2, gg.end());
                
                vector<G1> ww_L(ww.begin(), ww.begin()+ww.size()/2);
                vector<G1> ww_R(ww.begin()+ww.size()/2, ww.end());
                
                G2* pt1=&gg_L[0];

                GT l = MultiPairing(ww_R,gg_L);
                GT r = MultiPairing(ww_L,gg_R);

                L.insert(L.begin(),l);
                R.insert(R.begin(),r);

                string buf = this->P.getStr()+l.getStr()+r.getStr();
                Fr c;
                c.setHashOf(buf);

                x.insert(x.begin(),1/c);
                vector<G1> ww_hat(ww.size()/2);
                vector<G2> gg_hat(gg.size()/2);

                
                for(uint32_t i=0;i<ww_R.size();i++){

                    ww_hat[i]=ww_L[i]+(ww_R[i]*(c));
                    gg_hat[i]=gg_L[i]+(gg_R[i]*(1/c));
                    
                }
                
                GT Lx;
                GT::pow(Lx,l,c);
                GT Rinvx;
                GT::pow(Rinvx,r,(1/c));

                GT P_hat=Lx*P*Rinvx;

                Prove(gg_hat,P_hat,ww_hat);
                
            }
        }   
};

bool IPPverify(IPPproof pi);

struct zkIPPproof;

IPPproof zkIPPprove(vector<G2> gg, GT P, vector<G1> ww);

bool zkIPPverify(IPPproof pi);

#endif