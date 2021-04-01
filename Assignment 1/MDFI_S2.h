#include "MDFI.h"
#include <iostream>
#include <vector>
#include <mutex>
#include "dqueue.h"


class MDFI_S2: public MDFI{
    private:
        std::mutex mutex;
        int input_number;
        int id;
        std::vector<int> *vect;
        myqueue<std::vector<int>*> *collector_queue;
    
        void halfEven(){ 
            for(int i=0; i < vect->size(); i++){
                if((*vect)[i]%2==0)
                    (*vect)[i] = (*vect)[i] / 2;
            }
            collector_queue->push(vect);
        }

    public:
        MDFI_S2(int input_number, myqueue<std::vector<int>*> *out_queue) {
            id = id;
            input_number = input_number;
            collector_queue = out_queue;
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
            halfEven();
        }
};