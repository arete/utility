
#include <iostream>

#include "File.hh"

using namespace Utility;

int main (int argc, char* argv[])
{
  File f1 ("/usr/include");
  if (f1.Dirname() != "/usr")
    std::cerr << "Error 1" << std::endl;
  if (f1.Basename() != "include")
    std::cerr << "Error 2" << std::endl;
  if (f1.Extension() != "")
    std::cerr << "Error 2" << std::endl;

  File f2 ("test.mp3");
  if (f2.Dirname() != "")
    std::cerr << "Error 4" << std::endl;
  if (f2.Basename() != "test.mp3")
    std::cerr << "Error 5" << std::endl;
  if (f2.Extension() != "mp3")
    std::cerr << "Error 6" << std::endl;
 
}
