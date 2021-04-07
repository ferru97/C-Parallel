#include "Pool.h"
#include "MDFI_S2.h"
#include <iostream>
#include <mutex>
#include <atomic>
#include <unistd.h>

#ifndef MDFI_S1_H
#define MDFI_S1_H

class MDFI_S1: public MDFI{
    private:
        std::mutex mutex;
        std::vector<int> *vect;
        MDFI_S2 *son;
    
        void increaseVect(){ 
            usleep(100);
            for(int i=0; i < vect->size(); i++){
                (*vect)[i] = (*vect)[i] + 1;
            }
            son->setVector(vect);

        }

    public:

        MDFI_S1() {}
        
        MDFI_S1(MDFI_S2 *_s, int _id, Pool *_pool) {
            this->last_worker_id = -1;
            this->input_number = 1;
            this->pool = _pool;
            this->son = _s;
            this->id = _id;
        }

        void setVector(std::vector<int> *v){
            std::unique_lock<std::mutex> lock(mutex);
            vect = v;
            input_number -= 1;
            if(input_number==0){
                pool->push(this);
            }
        }

        void execFunction() override{
            increaseVect();
        }
};

#endif