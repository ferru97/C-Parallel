#include <chrono>
#include <thread>
#include "./utils/dqueue.h"

#define EOS -1

using namespace std::chrono_literals;


void drain(myqueue<int> &q);
void source(myqueue<int> &q);

int main(int argc, char *argv[]){
    myqueue<int> myq;

    std::thread tdrain(drain,std::ref(myq));
    std::thread tsource(source,std::ref(myq));

    tsource.join();
    tdrain.join();
}

void drain(myqueue<int> &q) {
  std::cout << "Drain started" << std::endl;
  auto e = q.pop();
  
  while(e != EOS) {
    std::cout << "received " << e << std::endl;
    e = q.pop();
  }
  return;
}


void source(myqueue<int> &q) {
    for(int i=0; i<16; i++){
        q.push(i);
        std::this_thread::sleep_for(1000ms);
    }
    q.push(EOS);
    std::cout << "sent EOS" << std::endl;
    return;
}

