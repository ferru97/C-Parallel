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
  int EOS_id;
  int out_threads_num;

public:
  myqueue() {}

  myqueue(int out_threads_num, T EOS_id) {
    this->out_threads_num = out_threads_num;
    this->EOS_id = EOS_id;
  }
  
  void push(T const& value) {
    {
      std::unique_lock<std::mutex> lock(d_mutex);
      if(value != EOS_id)
        d_queue.push_front(value);
      else{
        for(int i=0; i<this->out_threads_num; i++)
            d_queue.push_front(EOS_id);
      }
    }
    this->d_condition.notify_one();
  }
  
  T pop() {
    std::unique_lock<std::mutex> lock(d_mutex);
    d_condition.wait(lock, [=](){ return (!d_queue.empty() });  // wait if the queue is currently empty
    T rc(std::move(this->d_queue.back()));       // used to get the actual message rather than a copy
    d_queue.pop_back();                   // remove the item just read from the back end of the queue
    return rc;
  }

};