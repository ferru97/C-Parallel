#include <iostream>
#include <chrono>

class utimer {
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point stop;
  std::string message; 
  using usecs = std::chrono::microseconds;
  using msecs = std::chrono::milliseconds;

private:
  long * us_elapsed;
  
public:

  utimer(const std::string m) : message(m),us_elapsed((long *)NULL) {
    start = std::chrono::system_clock::now();
  }
    
  utimer(const std::string m, long * us) : message(m),us_elapsed(us) {
    start = std::chrono::system_clock::now();
  }

  ~utimer() {
    stop =std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = stop - start;
    
    auto musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();


    //std::cout << message << " computed in " << musec << " usec " << std::endl;
    std::cout << message << " computed in " << millis << " millis " << std::endl;
    if(us_elapsed != NULL)
      (*us_elapsed) = musec;
  }
};