#ifndef UTILITY__SPLITSTREAMBUFFER_HH__
#define UTILITY__SPLITSTREAMBUFFER_HH__

#include <ostream>

namespace Utility
{
  // a std::streambuffer implementation that forwards output to two
  // std::ostreams. Usage example (w.o. namespaces): 
  //
  // ostream (new SplitStreamBuffer (cout, logfile)) logstream;
  // logstream << "Hello world" << endl;

  class SplitStreamBuffer : public std::streambuf
  {
  public:
    SplitStreamBuffer (std::ostream& forward1, std::ostream& forward2)
      : fw1(forward1), fw2(forward2) { }

  protected:
    virtual int_type overflow (int_type c);

    std::ostream& fw1;
    std::ostream& fw2;
  };

} // end namespace Utility

#define UTILITY__SPLITSTREAMBUFFER_TMPL__
#include "template/SplitStreamBuffer.tcc"
#undef UTILITY__SPLITSTREAMBUFFER_TMPL__

#endif // UTILITY__SPLITSTREAMBUFFER_HH__
