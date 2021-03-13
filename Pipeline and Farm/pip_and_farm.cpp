#include <chrono>
#include <thread>
#include <sstream>
#include "../utils/utimer.h"
#include "../utils/dqueue_multi.h"

#define EOS -1
using namespace std::chrono_literals;

void source(myqueue<int> &q, int n_works, std::chrono::milliseconds wait);
void fun1(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait);
void fun2(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait);
void drain(myqueue<int> &in_q, std::chrono::milliseconds wait);

int main(int argc, char **argv){
    if(argc<3){
        std::cout << "Invlid arguments";
        return 0;
    }
    int farm_workers = std::stoi(argv[1]);
    int works = std::stoi(argv[2]);
    std::cout << "Using" << farm_workers*2+2 << "threada for " << works << " works" << std::endl;
    

    {
        utimer t("Pipeline and farm");

        myqueue<int> emitter_out(farm_workers, EOS);
        myqueue<int> drain_inq(1, EOS);

        std::vector<std::thread> threads;
        threads.reserve(farm_workers*2);


        std::thread source_t(source, std::ref(emitter_out), works, 10ms);
        std::thread drain_t(drain, std::ref(drain_inq), 10ms);
        for(int i=0; i<farm_workers; i=i+2){
            myqueue<int> tamp_queue(1, EOS);
            threads.emplace_back( std::thread {fun1, std::ref(emitter_out), std::ref(tamp_queue), 30ms});
            threads.emplace_back( std::thread {fun2, std::ref(tamp_queue), std::ref(drain_inq), 30ms});
        }

        source_t.join();
        drain_t.join();
        for(std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it) 
            it->join();
    }
    
    return 0;
}

void source(myqueue<int> &q, int n_works, std::chrono::milliseconds wait){
    for(int i=0; i<n_works; i++){
        std::this_thread::sleep_for(wait);
        q.push(i);
        std::cout << "Source emit " << i << std::endl; 
    }

    q.push(EOS);
    std::cout << "Emitter send EOS" << std::endl;
}

void fun1(myqueue<int> &in_q, myqueue<int> &out_q, std::chrono::milliseconds wait){
    int work = in_q.pop();
    while(work != EOS){
        std::this_thread::sleep_for(wait);
        work = work + 1;
        out_q.push(work);
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
    }

    out_q.push(EOS);
    std::cout << "F1 send EOS" << std::endl;
}

void drain(myqueue<int> &in_q, std::chrono::milliseconds wait){
    int work = in_q.pop();
    while(work != EOS){
        std::this_thread::sleep_for(wait);
        work = work * 2;
    }

    std::cout << "F2 send EOS" << std::endl;
}