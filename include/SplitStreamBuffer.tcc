#ifndef UTILITY__SPLITSTREAMBUFFER_TMPL__
#error "This file is included by .... "
#error "Include that file instead." 
#endif


namespace Utility
{

  inline std::streambuf::int_type SplitStreamBuffer::overflow (int_type c)
  {
    if (c != EOF)
      {
	fw1.put (c);
	fw2.put (c);
      }
    return c;
  }

} // end namespace Utility

