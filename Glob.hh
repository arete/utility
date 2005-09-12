
#ifndef UTILITY__GLOB_HH__
#define UTILITY__GLOB_HH__

#include <glob.h>

namespace Utility
{

/* very lightweight glob wrapper ... */
class Glob
{
 public:

  typedef char** iterator;

  Glob (const std::string& pattern) {
    glob (pattern.c_str(), 0, NULL, &g);
  }
  ~Glob () {
    globfree (&g);
  }

  iterator begin() {
    return g.gl_pathv;
  }

  iterator end() {
    return g.gl_pathv + g.gl_pathc;
  }
  
  operator void*() {
    return g.gl_pathc == 0 ? 0 : this;
  }
  
  bool operator!() const {
    return g.gl_pathc == 0;
  }
  
  private:
    glob_t g;
};

}

#endif
