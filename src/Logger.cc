#include "Logger.hh"
#include <time.h>
#include <iostream>

namespace Utility
{ // Exports
  bool basic_log_device_initialized = false;
  extern BasicLogDevice::timestamp_t BasicLogDevice::current;
  extern BasicLogDevice::direction BasicLogDevice::last_direction;
  extern BasicLogDevice* BasicLogDevice::last_device;
  std::ostream& wrap_cout = std::cout;
  std::ostream& wrap_cerr = std::cerr;
}

using namespace Utility;

BasicLogDevice::BasicLogDevice ()
{
  if (!basic_log_device_initialized)
    {
      current = true;
      last_device = this;
      basic_log_device_initialized = true;
    }
};



void BasicLogDevice::PrintPrelude ()
{
  time_t t;
  time (&t);
  (*log_file) << "log opened " << ctime(&t) << std::endl;
}

void BasicLogDevice::PrintTimeStamp (std::ostream* str)
{
  (*str) << "[@t" << current++ << "] ";
}


void BasicLogDevice::AllocateSplitStreams ()
{
  split_buffer_cout = new SplitStreamBuffer (std::cout, *log_file);
  split_buffer_cerr = new SplitStreamBuffer (std::cerr, *log_file);
  split_stream_cout = new std::ostream (split_buffer_cout);
  split_stream_cerr = new std::ostream (split_buffer_cerr);
}

  

void BasicLogDevice::DeallocateSplitStreams ()
{
  delete split_stream_cout;
  delete split_stream_cerr;
  delete split_buffer_cout;
  delete split_buffer_cerr;
}

