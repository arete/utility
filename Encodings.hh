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
   
    int pad = 0;
    uint32_t v = (uint32_t)data[i + 0] << 16;
    if (i + 1 < length) {
      v |= (uint32_t)data[i + 1] << 8;
      if (i + 2 < length)
	v |= (uint32_t)data[i + 2] << 0;
      else
	pad = 1;
    } else
      pad = 2;
    
    stream.put(base64lookup[v >> 18 & 63]);
    stream.put(base64lookup[v >> 12 & 63]);
    stream.put(pad > 1 ? '=' : base64lookup[(v >> 6 & 63)]);
    stream.put(pad > 0 ? '=' : base64lookup[(v >> 0 & 63)]);
  }
}

template <typename T>
void DecodeBase64(std::ostream& stream, const T& data, size_t length)
{
  static const char base64lookup[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  for (size_t i = 0; i < length; ) {
    uint32_t v = 0;
    int skip = 0;
    
    for (int j = 4; i < length && j > 0;) {
      char c = data[i++];
      // this is a bit sloppy and also skips for '=' in the middle of the
      // stream, but those are not well formed anyway
      if (c == '=') {
	++skip; ++j;
	continue;
      }
      
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
    if (skip < 2)
      stream.put((char)((v >>  8) & 0xFF));
    if (skip < 1)
      stream.put((char)((v >>  0) & 0xFF));
  }
}

#ifdef WITHZLIB

#include <zlib.h>

// TODO: make template, likewise
inline bool EncodeZlib (std::ostream& stream, const char* data, size_t length, int level = 9)
{
  static const unsigned CHUNK = 16384;
  
  z_stream strm;
  char out[CHUNK];
  
  /* allocate deflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  int ret = deflateInit(&strm, level);
  if (ret != Z_OK)
    return false;
  
  /* compress until end of file */
  for (size_t i = 0; i < length; i += CHUNK) {
    strm.avail_in = (length - i > CHUNK) ? CHUNK : (length - i);

    const int flush = ((length - i - strm.avail_in) == 0 ? Z_FINISH : Z_NO_FLUSH);
    strm.next_in = (Bytef*) &data[i];
    
    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */
    do {
      strm.avail_out = CHUNK;
      strm.next_out = (Bytef*) out;
      ret = deflate(&strm, flush);    /* no bad return value */
      unsigned have = CHUNK - strm.avail_out;
      stream.write(out, have);
      if (!stream) {
	(void)deflateEnd(&strm);
	return false;
      }
    } while (strm.avail_out == 0);
  }

  /* clean up and return */
  (void)deflateEnd(&strm);
  return true;
}

#endif

#endif
