#include <thread>
#include <chrono>
#include <stack>
#include <time.h> 
#include <iostream>
#include <stdlib.h>
#include "MDFI_S1.h"
#include "MDFI_S2.h"
#include "Worker.h"
#include "Pool.h"
#include "../utils/utimer.h"

Pool pool;
std::vector<Worker*> workers;

void createMDFG(int n_pairs, int max_n, int max_m);
void poolManager(int total_works, int use_affinity);

int main(int argc, char **argv){
    if(argc<7){
        std::cout << "Invlid arguments: provide n_pairs, max_n, max_m, random_seed, thread_num, use_affinity(0-1)" <<std::endl;
        return 0;
    }

    int n_pairs = std::stoi(argv[1]);
    int max_n = std::stoi(argv[2]);
    int max_m = std::stoi(argv[3]);
    int random_seed = std::stoi(argv[4]);
    int nw = std::stoi(argv[5]);
    int use_affinity = std::stoi(argv[6]);
    srand(random_seed);

    {
        utimer timer("Work");
        for(int i=0; i<nw; i++){
            Worker *w = new Worker();
            w->spawn();
            workers.push_back(w);
        }
        
        std::thread manager(poolManager, n_pairs*3, use_affinity);
        std::thread emitter(createMDFG, n_pairs, max_n, max_m);
        manager.join();
        emitter.join();

        for(int i=0; i<workers.size(); i++){
            workers[i]->stopThread();
        }
    }

    return 0;
}

void poolManager(int total_works, int use_affinity){
    MDFI *work;
    bool worker_found = false;
    int work_done = 0;
    while(work_done!=total_works){
        work = pool.pop();
        if(use_affinity==1 && work->getLastWorkerID()>=0){
            workers[work->getLastWorkerID()]->setWork(work);
            work_done += 1;
        }else{
            worker_found = false;
            while(!worker_found){
                for(int i=0; i<workers.size(); i++){
                    if(workers[i]->isAvailable()){
                        work->setLastWorkerID(i);
                        workers[i]->setWork(work);
                        worker_found = true;
                        work_done += 1;
                        break;
                    }
                }
            }
        }
    }
}

void createMDFG(int n_pairs, int max_n, int max_m){
    int id = 0;

    for(int i=0; i<n_pairs; i++){
        int rand_n = rand() % max_n + 1;
        std::vector<int> *vect = new std::vector<int>(rand_n);
        for(int k=0; k<rand_n; k++)
            (*vect)[k] = rand() % max_m;

        MDFI_S3 *stage3 = new MDFI_S3(&pool, id);
        id += 1;
        MDFI_S2 *stage2 = new MDFI_S2(stage3, id, &pool); 
        id += 1;
        MDFI_S1 *stage1 = new MDFI_S1(stage2, id, &pool);  
        id += 1;

        stage1->setVector(vect);
    }
}