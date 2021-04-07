#include "Pool.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <stack>
#include <unistd.h>

#ifndef MDFI_S3_H
#define MDFI_S3_H

class MDFI_S3: public MDFI{
    private:
        std::mutex mutex;
        std::vector<int> *vect;
    
        void printVector(){ 
            usleep(300);
            std::cout << "\nOutput" << std::endl;
            for(int i=0; i < vect->size(); i++){
                std::cout<< (*vect)[i] << std::flush;
            }
        }

    public:
        MDFI_S3(Pool *_pool, int _id) {
            this->last_worker_id = -1;
            id = _id;
            pool = _pool;
            input_number = 1;
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
            printVector();
        }
};

#endif