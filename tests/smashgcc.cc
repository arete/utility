#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Obj
{
public:
  void Foo ();
};

namespace N
{
  template <typename T>
  class SomeFunctor
  {
  public:
    void operator () (T& t) {t -> Foo ();}
  };
}

main ()
{


  vector<Obj*> v;
  vector<Obj*>::iterator begin = v.begin ();
  vector<Obj*>::iterator end = v.end ();

  // syntactical error crashes g++ :
  for_each (begin, end, N::SomeFunctor);
  // should be :
  // for_each (begin, end, N::SomeFunctor <Obj*> () );
}
