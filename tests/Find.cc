
#include <iostream>
#include <sstream>

#include "DirIterator.hh"

void scan (const std::string s)
{
  Utility::DirList dir (s);
  Utility::DirList::Iterator it (dir.Begin());
  Utility::DirList::Iterator it_end (dir.End ());
  
  while  (it != it_end) {
    std::cout << s + '/' + *it << std::endl;
    if (it.Type().IsDirectory())
      scan (s + '/' + *it);
    ++ it;
  }

}

int main (int argc, char* argv[])
{
  std::string start ("/home/rene");
  if (argc > 1)
    start = argv[1];
  
  scan (start);
}
