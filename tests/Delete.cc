#include "Delete.hh"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Obj
{
public:
  Obj () {cout << "ctor" << endl;}
  ~Obj () {cout << "dtor" << endl;}
};

main ()
{
  vector<Obj*> v;
  v.push_back (new Obj ());
  v.push_back (new Obj ());

  vector<Obj*>::iterator begin = v.begin ();
  vector<Obj*>::iterator end = v.end ();

  for_each (begin, end, Utility::DelFunctor<Obj*> ());
}
