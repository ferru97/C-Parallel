#include "MDFI.h"
#include "MDFI_S2.h"
#include <iostream>
#include <mutex>
#include <atomic>

class MDFI_S1: public MDFI{
    private:
        std::mutex mutex;
        int input_number;
        int id;
        std::vector<int> *vect;
        MDFI_S2 *son;
    
        void increaseVect(){ 
            for(int i=0; i < vect->size(); i++){
                (*vect)[i] = (*vect)[i] + 1;
            }
            son->setVector(vect);
        }

    public:
        MDFI_S1(MDFI_S2 *s, int id) {
            input_number = 1;
            son = s;
            id = id;
        }

        void setVector(std::vector<int> *v){
            std::unique_lock<std::mutex> lock(mutex);
            vect = v;
            input_number -= 1;
            if(input_number==0){
                //schedule
            }
        }

        void execFunction(){
            increaseVect();
        }
};