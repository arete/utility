
#include <iostream>

#include "Threads.hh"

class TestThread : public Utility::Threads::Thread
{
 public:
  TestThread () {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  ~TestThread () {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

 protected:

  void* main (void* obj) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << "In thread" << std::endl;
  }

};

int main ()
{
  TestThread thread;
  thread.start (0);

  thread.join ();

  return 0;
}

