
#include "Attribute.hh"

int main ()
{
  GSMP::Attribute<int, true> a_with_signal;
  GSMP::Attribute<int, false> a_without_signal;
  int x;
  x = a_with_signal.Get ();
  x = a_without_signal.Get ();
  return 0;
}
