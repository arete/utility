
#include "Attribute.hh"

class foo
{
 public:
  const Utility::Attribute<int>& bar;

  foo ();

 private:
  Utility::Attribute<int> real_bar;
};

foo::foo ()
  : bar (real_bar)
{
}

int main ()
{
  Utility::Attribute<int, true> a_with_signal;
  Utility::Attribute<int, false> a_without_signal;
  int x;
  x = a_with_signal.Get ();
  x = a_without_signal.Get ();

  foo f;
  f.bar.Get ();

  return 0;
}
