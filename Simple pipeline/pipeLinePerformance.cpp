#include <chrono>
#include <thread>
#include "./utils/dqueue.h"
#include "./utils/utimer.h"
#define EOS -1

using namespace std::chrono_literals;


void source(myqueue<int> &q, int n, std::chrono::milliseconds msecs);
void genericstage(myqueue<int> &inq, myqueue<int> &outq, std::chrono::milliseconds msecs);
void drain(myqueue<int> &q, std::chrono::milliseconds msecs);

int main(int argc, char *argv[]){
    myqueue<int> source2one, one2drain;

    auto inc = [](int x) { return(x+1);};

    {
      utimer t("Test");
      std::thread s1(source, std::ref(source2one), 10, 1000ms);
      std::thread s2(genericstage, std::ref(source2one), std::ref(one2drain), 5000ms);
      std::thread s22(genericstage, std::ref(source2one), std::ref(one2drain), 5000ms);
      std::thread s3(drain, std::ref(one2drain), 1000ms);

      s1.join();
      s2.join();
      s22.join();
      s3.join();
    }
}


void source(myqueue<int> &q, int n, std::chrono::milliseconds msecs) {
    for(int i=0; i<n; i++){
        std::this_thread::sleep_for(msecs);
        q.push(i);
        std::cout << "Drain emitted " << i << std::endl; 
    }
    q.setEOSemitted();
    std::cout << "sent EOS" << std::endl;
    return;
}

void genericstage(myqueue<int> &inq, myqueue<int> &outq, std::chrono::milliseconds msecs) {
  try
  {
    auto e = inq.pop();
    while(1){
      std::this_thread::sleep_for(msecs);
      auto res = e+1;
      outq.push(res);
      e = inq.pop();
    };
  }
  catch (std::out_of_range e){}

  outq.setEOSemitted();
  return;
}

void drain(myqueue<int> &q, std::chrono::milliseconds msecs) {
  std::cout << "Drain started" << std::endl;

  try
  {
    auto e = q.pop();
    while(1) {
      std::this_thread::sleep_for(msecs);
      std::cout << "received " << e << std::endl;
      e = q.pop();
    };
  }
  catch (std::out_of_range e){}

  return;
}
