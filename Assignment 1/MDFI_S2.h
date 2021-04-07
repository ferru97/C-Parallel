#include "Pool.h"
#include "MDFI_S3.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <stack>
#include <unistd.h>

#ifndef MDFI_S2_H
#define MDFI_S2_H

class MDFI_S2: public MDFI{
    private:
        std::mutex mutex;
        std::vector<int> *vect;
        MDFI_S3 *son;
    
        void halfEven(){ 
            usleep(200);
            for(int i=vect->size()-1; i >= 0; i--){
                if((*vect)[i]%2==0)
                    (*vect)[i] = (*vect)[i] / 2;
            }
            son->setVector(vect);
        }

    public:
        MDFI_S2(MDFI_S3 *_s, int _id, Pool *_pool) {
            this->last_worker_id = -1;
            input_number = 1;
            id = _id;
            pool = _pool;
            son = _s;
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
            halfEven();
        }
};

#endif