/*
 * Copyright (c) 2008 Valentin Ziegler <valentin@exactcode.de>
 *
 */

#ifndef ASCII85_HH
#define ASCII85_HH

#include <ostream>

/* 
   All encoding functions have signature 
   
   void Encode... (std::ostream& stream, const T& data, size_t length ....)

   Type T has to implement const CH& operator[] (int) const
   where CH can be casted into uint8_t. Most likely T will be one out of
   uint8_t*, std::vector<uint8_t>, std::string.
*/


template <typename T>
void EncodeHex(std::ostream& stream, const T& data, size_t length)
{
  const int limit=40;
  static const char nibble[] = "0123456789abcdef";

  for (size_t i = 0; i < length; ++i) {
    if (i % limit == 0 && i != 0)
      stream.put('\n');
    uint8_t byte=data[i];
    stream.put(nibble[byte >> 4]);
    stream.put(nibble[byte & 0x0f]);
  }
}

template <typename T>
void EncodeASCII85(std::ostream& stream, const T& data, size_t length)
{
  const int limit=80;
  int bytes=0;
  if (length>0) {
    length--; // decrementing length allows comparisons below without
              // performing additional arithmetics
    uint32_t quad=0;
    int count=3;
    for (size_t i=0; i<=length; i++) {
      quad <<= 8;
      quad |= (uint8_t) data[i];
      if (i==length || count==0) {
	if (i==length) // pad with zeroes if necessary
	  for (int n=count; n>0; --n)
	    quad <<= 8;

	if (!(count || quad)) {
	  stream.put('z');
	  if(++bytes==limit) {stream.put('\n'); bytes=0;}
	} else {
	  char seq[5];
	  int n;
	  for (n=4; n>=0; n--) {
	    uint32_t rem=quad % 85;
	    quad /= 85;
	    seq[n]='!'+rem;
	  }
	  for (n=0; n<5-count; n++) {
	    stream.put(seq[n]);
	    if(++bytes==limit) {stream.put('\n'); bytes=0;}
	  }
	}
	quad=0;
	count=4;
      }
      count--;
    }
  }
  if (bytes>=limit-1)
    stream.put('\n');
  stream << "~>";
}

#endif
