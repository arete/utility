
#ifndef UTILITY__FOREACHIF_HH__
#define UTILITY__FOREACHIF_HH__

namespace Utility
{

  template<typename ITER, typename PRED, typename FUNC>
  FUNC for_each_if (ITER first, ITER last, PRED p, FUNC f)
  {
    for ( ; first != last; ++first )
      if ( p ( *first ) )
	f (*first);
    return f;
  }


  template<typename ITER, typename FUNC>
  bool for_each_noerror (ITER first, ITER last, FUNC f)
  {
    bool success = true;
    for ( ; first != last && success; ++first )
      success &= f (*first);
    return success;
  }
  
  template<typename ITER, typename PRED, typename FUNC>
  bool for_each_if_noerror (ITER first, ITER last, PRED p, FUNC f)
  {
    bool success = true;
    for ( ; first != last && success; ++first )
      if ( p ( *first ) )
	success &= f (*first);
    return success;
  }



} // end namespace Utility


#endif // UTILITY__FOREACHIF_HH__
