#include "Logger.hh"

using namespace Utility;

bool Utility::basic_log_device_initialized = false;



BasicLogDevice::BasicLogDevice ()
{
  if (!basic_log_device_initialized)
    {
      current = true;
      last_device = this;
      basic_log_device_initialized = true;
    }
};


