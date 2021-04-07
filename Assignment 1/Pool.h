#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stack>

#ifndef POOL_H
#define POOL_H

class MDFI;
class Pool;

class MDFI{

    public:
        int input_number;
        int last_worker_id;
        int id;
        Pool *pool;
        
        MDFI(){}

        virtual void execFunction(){ 
            std::cout << "Hello world" << std::endl;
        }

        int getLastWorkerID(){ return last_worker_id; }
        void setLastWorkerID(int wid){ last_worker_id = wid; }
};

class Pool{
    private:
        std::mutex mutex;
        std::stack<MDFI*> pool;
        std::condition_variable d_condition;

    public:

        Pool(){}

        void push(MDFI *value) {
            {
            std::unique_lock<std::mutex> lock(mutex);
            pool.push(value);
            }
            this->d_condition.notify_one();
        }
            
        MDFI* pop() {
            std::unique_lock<std::mutex> lock(mutex);
            d_condition.wait(lock, [=](){ return !pool.empty(); });
            MDFI* val = pool.top();
            pool.pop();
            return val;
        }
};

#endif