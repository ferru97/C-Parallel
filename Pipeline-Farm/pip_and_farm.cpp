#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>
#include <list>
#include "../utils/utimer.h"
#include "../utils/dqueue_multi.h"

#define EOS -1
using namespace std::chrono_literals;

void source(myqueue<int> &q, int n_works, std::chrono::milliseconds wait);
void fun1(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait);
void fun2(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait);
void drain(myqueue<int> &in_q, std::chrono::milliseconds wait);

int main(int argc, char **argv){
    std::cout << "In this example whe have 4 stages, each stage takes a thread " << std::endl;
    std::cout << std::endl << "Stage1(10ms) -> Stage2(30) -> Stage3(50ms) -> stage4(10ms), the farm parallelize stages 2 and 3 " << std::endl;

    if(argc<3){
        std::cout << "Invlid arguments: ./out farm_instances num_works";
        return 0;
    }
    int farm_workers = std::stoi(argv[1]);
    int works = std::stoi(argv[2]);
    std::cout << std::endl <<  "Using " << farm_workers*2+2 << " threadas for " << works << " works" << std::endl;
    

    {
        utimer t("Pipeline and farm");

        myqueue<int> emitter_out(1, farm_workers, EOS);
        myqueue<int> drain_inq(farm_workers, 1, EOS);

        std::list<std::thread> threads;

        std::thread source_t(source, std::ref(emitter_out), works, 10ms);
        std::thread drain_t(drain, std::ref(drain_inq), 10ms);
        for(int i=0; i<farm_workers; i=i+1){
            myqueue<int> tamp_queue(1,1, EOS);
            threads.push_back( std::thread {fun1, std::ref(emitter_out), std::ref(tamp_queue), 30ms});
            threads.push_back( std::thread {fun2, std::ref(tamp_queue), std::ref(drain_inq), 50ms});
        }

        std::list<std::thread>::iterator it;
        source_t.join();
        for (it = threads.begin(); it != threads.end(); ++it)
            it->join();
        drain_t.join();

    }
    
    return 0;
}

void source(myqueue<int> &q, int n_works, std::chrono::milliseconds wait){
    for(int i=0; i<n_works; i++){
        std::this_thread::sleep_for(wait);
        q.push(i);
        //std::cout << "Source emit " << i << std::endl; 
    }

    q.push(EOS);
    std::cout << "Source send EOS" << std::endl;
}

void fun1(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait){
    int work = in_q.pop();
    while(work != EOS){
        std::this_thread::sleep_for(wait);
        work = work + 1;
        out_q.push(work);
        work = in_q.pop();
    }

    out_q.push(EOS);
    std::cout << "F1 send EOS" << std::endl;
}

void fun2(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait){
    int work = in_q.pop();
    while(work != EOS){
        std::this_thread::sleep_for(wait);
        work = work * 2;
        out_q.push(work);
        work = in_q.pop();
    }

    out_q.push(EOS);
    std::cout << "F2 send EOS" << std::endl;
}

void drain(myqueue<int> &in_q, std::chrono::milliseconds wait){
    int work = in_q.pop();
    while(work != EOS){
        //std::cout << "Drain receive " << work << std::endl;
        std::this_thread::sleep_for(wait);
        work = work * 2;
        work = in_q.pop();
    }

    std::cout << "Drain END" << std::endl;
}