#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "../utils/utimer.h"

using namespace std::chrono_literals;


int main(int argc, char **argv){
    if(argc<2){
        std::cout << "Invlid arguments: ./out parallelism_degree array_size [skip_sequential]";
        return 0;
    }
    int parall_degree = std::stoi(argv[1]);
    int vector_size = std::stoi(argv[2]);
    std::vector<int> vect(vector_size, 10);

    auto inc = [](int num){ 
        std::this_thread::sleep_for(10ms);
        return num+1; 
    };

    std::string t = "skip_sequential";
    if(argc>2 && t.compare(argv[3])==0){
        std::cout << "skip sequential computation" << std::endl;
    }else{
        utimer t("Sequential Time");

        for(int i=0; i<vect.size(); i++){
            vect[i] = inc(vect[i]);
        }
    }

    {
        utimer t("Parallel Time");

        #pragma omp parallel for num_threads(parall_degree) schedule(static)
        for(int i=0; i<vect.size(); i++){
            vect[i] = inc(vect[i]);
        }
    }
}