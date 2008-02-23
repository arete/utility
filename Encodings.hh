/*
 * Copyright (c) 2008 Valentin Ziegler <valentin@exactcode.de>
 * Copyright (c) 2008 René Rebe <rene@exactcode.de>
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

template <typename T>
void EncodeBase64(std::ostream& stream, const T& data, size_t length)
{
  const int limit = 57;
  static const char base64lookup[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  for (size_t i = 0; i < length; i += 3) {
    if (i % limit == 0 && i != 0)
      stream.put('\n');
   
    uint32_t v = (      (uint32_t)data[i + 0] << 16   ) |
      (i + 1 < length ? (uint32_t)data[i + 1] << 8 : 0) |
      (i + 2 < length ? (uint32_t)data[i + 2] << 0 : 0);
    
    stream.put(base64lookup[v >> 18 & 63]);
    stream.put(base64lookup[v >> 12 & 63]);
    stream.put(base64lookup[v >> 6 & 63]);
    stream.put(base64lookup[v >> 0 & 63]);
  }
}

template <typename T>
void DecodeBase64(std::ostream& stream, const T& data, size_t length)
{
  static const char base64lookup[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  for (size_t i = 0; i < length; ) {
    uint32_t v = 0;
    
    for (int j = 4; i < length && j > 0;) {
      char c = data[i++];
      // valid base64 content? (skip \n\r et al.)
      unsigned int k;
      for (k = 0; k < sizeof(base64lookup) - 1; ++k)
	if (c == base64lookup[k])
	  break;
      if (k != sizeof(base64lookup) - 1) {
	--j;
	//std::cerr << j << ": c: " << c << " to " << k << " for " << std::endl;
	v |= k << (6 * j);
      }
    }
    
    stream.put((char)((v >> 16) & 0xFF));
    stream.put((char)((v >>  8) & 0xFF));
    stream.put((char)((v >>  0) & 0xFF));
  }
}

#endif
