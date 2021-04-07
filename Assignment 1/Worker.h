#include <thread>
#include <condition_variable>
#include "Pool.h"

class Worker{
    private:
        std::thread* t;
        std::mutex mutex;
        std::condition_variable condition;
        MDFI *work;
        bool stop;
    
    public:

        Worker(){
            work = NULL;
            stop = false;
        }

        void setWork(MDFI *w){
            std::unique_lock<std::mutex> lock(mutex);
            work = w;
            condition.notify_one();
        }

        void spawn(){
            t = new std::thread( [this] (){ 
                while(!this->stop){
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this](){ return this->work!=NULL || this->stop; });
                    if(!this->stop){
                        this->work->execFunction();
                        this->work = NULL;
                    }
                }
                return;
            } );
        }

        bool isAvailable(){
            return work == NULL;
        }

        void stopThread(){
            this->stop = true;
            condition.notify_one();
            this->t->join();
        }

};