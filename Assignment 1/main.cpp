#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char **argv){

    if(argc<5){
        std::cout << "Invlid arguments: provide n_pairs, max_n, max_m, random_seed";
        return 0;
    }

    int n_pairs = std::stoi(argv[1]);
    int max_n = std::stoi(argv[2]);
    int max_m = std::stoi(argv[3]);
    int random_seed = std::stoi(argv[4]);


}