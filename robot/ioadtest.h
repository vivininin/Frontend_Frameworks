#pragma once
#define For(i, n) for(int i = 0; i < (int)(n); i++)
#include "mission.h"

// io/ad test
class IOADTest:public Mission{
    private:
        int cnt1, cnt2, patt;
    public:
        IOADTest(int adCount, int ioCount, int ioPattern){
            cnt1 = adCount; cnt2 = ioCount; patt = ioPattern;
        }
        void init(){
            For(i, cnt1) MFADEnable(i);
            MFADInit(100);
        }
        void step(double dt){
            if(cnt2){
                printf("AD: "); For(i, cnt2) printf("%d", i%10); printf("\n");
                printf("    "); For(i, cnt2) printf("%d", MFGetAD(i)); printf("\n");
            }

            if(cnt1){
                printf("IO: "); For(i, cnt1) printf("%d", i%10); printf("\n");
                printf("    "); For(i, cnt1) printf("%d", MFGetDigiInput(i)); printf("\n");
            }
        }
};

