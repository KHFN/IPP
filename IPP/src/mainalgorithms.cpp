#include <mcl/bls12_381.hpp>
#include <iostream>
#include <vector>
#include "polyonfr.hpp"

using namespace mcl;
using namespace mcl::bn;
using namespace std;
struct pcs;

template <typename T>
void putG(vector<T> a){
    for(uint32_t i=0;i<a.size();i++){
        cout<<"<"<<a[i].getStr(10).c_str()<<">"<<endl;
    }
}

GT MultiPairing(vector<G1> a, vector<G2> b){
    if(a.size()!=b.size()){
        throw runtime_error("Two vectors must have same length");
    }

    uint32_t n=a.size();
    G1* a_arr=&a[0]; G2* b_arr=&b[0];
    GT c;
    millerLoopVec(c,a_arr,b_arr,n);
    finalExp(c,c);

    return c;
}

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

bool IPPverify(IPPproof pi){

    uint32_t l = pi.L.size();
    FrVec x;

    GT P_v=pi.P;
    GT Lx;
    GT Rxinv;

    for(uint32_t i=0;i<l;i++){
        string buf = pi.P.getStr()+pi.L[i].getStr()+pi.R[i].getStr();
        Fr c;
        c.setHashOf(buf);
        x.push_back(1/c);
        GT::pow(Lx,pi.L[i],c);
        GT::pow(Rxinv,pi.R[i],1/c);
        P_v=Lx*P_v*Rxinv;
    }

    uint32_t n=pi.gg.size();
    uint32_t n_l=nextPowOf2(log2(n));
    vector<FrVec> M(n_l, FrVec((n>>1)+1,0));
    for(uint32_t i=0;i<n_l;i++){
        if(i<log2(n)){
            uint32_t j=1<<(i);
            M[i][0]= 1;
            M[i][j]= x[i];
        }else{
            M[i].resize(1);
            M[i][0]=1;
        }
    }

    FrVec u;
    FrVec v;
    int64_t index;
    index=0;
    uint8_t tmp=log2(n_l);
    for(uint8_t i=1;i<=tmp;i++){
        uint32_t L = (1<<(tmp-i));
        vector<FrVec> m(L, FrVec((n>>1)+1,0));
        for(uint64_t j=0;j<L;j++){
            u=M[index];
            index++;
            v=M[index];
            index++;
            m[j]=PolyMul(u,v);
        }
        index = 0;
        M=m;

    }

    G2* ptgg=&pi.gg[0];
    Fr* ptM=&M[0][0];

    
    G2 g_v;
    G2::mulVec(g_v,ptgg,ptM,pi.gg.size());

    GT res;
    pairing(res,pi.w,g_v);

    bool flag=(P_v==res);

    return flag;

}

struct zkIPPproof{
    
    IPPproof pi;
    GT Q;
    GT R;

    void Prove(vector<G2> gg, GT P, vector<G1> ww){
        
        vector<G1> vv(gg.size());
        for(uint32_t i=0;i<ww.size();i++){

            Fr c;
            c.setByCSPRNG();
            G1 v_i;
            hashAndMapToG1(v_i,c.getStr());
            vv[i]=v_i;

        }
        this->Q=MultiPairing(vv,gg);
        Fr chal;
        chal.setHashOf(Q.getStr());
        vector<G1> uu(ww.size());
        
        for(uint32_t i=0;i<ww.size();i++){
            uu[i]=(ww[i]*chal)+vv[i];
        }

        GT P_c;
        GT::pow(P_c,P,chal);
        this->R=P_c*(this->Q);
        
        IPPproof pi_zk(gg,P);
        pi_zk.Prove(gg,R,uu);
        this->pi=pi_zk;
        pi_zk.P=P;

    }

};

IPPproof zkIPPprove(vector<G2> gg, GT P, vector<G1> ww)
{
    vector<G1> vv(gg.size());
        for(uint32_t i=0;i<ww.size();i++){

            Fr c;
            c.setByCSPRNG();
            G1 v_i;
            hashAndMapToG1(v_i,c.getStr());
            vv[i]=v_i;

        }
        GT Q=MultiPairing(vv,gg);
        Fr chal;
        chal.setHashOf(Q.getStr());
        vector<G1> uu(ww.size());
        
        for(uint32_t i=0;i<ww.size();i++){
            uu[i]=(ww[i]*chal)+vv[i];
        }
        
        GT P_c;
        GT::pow(P_c,P,chal);
        GT R=P_c*Q;
        IPPproof pi_zk(gg,R);
        pi_zk.Q=Q;
        pi_zk.Prove(gg,R,uu);
        pi_zk.P=P;

        return pi_zk;
}

bool zkIPPverify(IPPproof pi){

    Fr chal;
    chal.setHashOf(pi.Q.getStr());
    GT P_c;
    GT::pow(P_c,pi.P,chal);
    GT R=P_c*pi.Q;
    pi.P=R;

    bool flag=IPPverify(pi);
    return flag;
}


