
#include "Compiler.hh"

#if GCC_VERSION < 30400 || __INCLUDE_LEVEL__ == 1

#ifndef UTILITY__ARGUMENTLIST_TCC__
#define UTILITY__ARGUMENTLIST_TCC__

namespace Utility {
  
  template <> bool Argument<bool>::Read () {
    // the ctor garuantees we have one value!
    values[0] = true;
  ++ count;
  return true;
  }
  
  template <> bool Argument<bool>::Read (const std::string& arg) {
    std::cout << "Error: No parameter allowed for boolean values!" << std::endl;
    return false;
  }
  
} // namespace Utility

#endif // UTILITY__ARGUMENTLIST_TCC__

#endif

