
#include "Attribute.hh"

int main ()
{
  GSMP::Attribute<int, true> a_with_signal;
  GSMP::Attribute<int, false> a_without_signal;
  return 0;
}
