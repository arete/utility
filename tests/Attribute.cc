
#include "Attribute.hh"

int main ()
{
  Utility::Attribute<int, true> a_with_signal;
  Utility::Attribute<int, false> a_without_signal;
  int x;
  x = a_with_signal.Get ();
  x = a_without_signal.Get ();
  return 0;
}
