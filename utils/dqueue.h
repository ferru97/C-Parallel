#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <chrono>
#include <cstddef>
#include <math.h>
#include <string>

template <typename T>
class myqueue
{
private:
  std::mutex              d_mutex;
  std::condition_variable d_condition;
  std::deque<T>           d_queue;
public:
  bool EOS_emitted;

  myqueue(std::string s) { 
    std::cout << "Created " << s << " queue " << std::endl;
    EOS_emitted = false;
  }
  myqueue() {}
  
  void push(T const& value) {
    {
      std::unique_lock<std::mutex> lock(d_mutex);
      d_queue.push_front(value);
    }
    this->d_condition.notify_one();
  }
  
  T pop() {
    std::unique_lock<std::mutex> lock(d_mutex);
    d_condition.wait(lock, [=](){ return (!d_queue.empty() || this->EOS_emitted); });  // wait if the queue is currently empty
    if(!EOS_emitted)
      throw std::out_of_range("END");
    T rc(std::move(this->d_queue.back()));       // used to get the actual message rather than a copy
    d_queue.pop_back();                   // remove the item just read from the back end of the queue
    return rc;
  }

  void setEOSemitted(){
    this->EOS_emitted = true;
  }

};