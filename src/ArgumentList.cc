
#include <sstream>

#include "ArgumentList.hh"

BasicArgument::BasicArgument (const std::string& i_sname, const std::string& i_lname,
			      const std::string& i_desc,  int i_min_count, int i_max_count)
  : count (0)
{
  min_count  = i_min_count;
  max_count = i_max_count;
  
  sname = i_sname;
  lname = i_lname;
  desc = i_desc;
  
  if (min_count > max_count)
    max_count = min_count;
  needs_arg = max_count != 0;
}

BasicArgument::~BasicArgument ()
{
}

void ArgumentList::Add (BasicArgument* arg)
{
  short_content [arg->sname] = arg;
  long_content [arg->lname] = arg;
}
  
bool ArgumentList::Read (int argc, char** argv)
{
  char** argv_end = argv + argc;
  int errors = 0;
  
  // skip the program name ...
  ++argv;

  BasicArgument* argument = 0;
  
  // parse all arguments
  while (argv != argv_end) {
    std::string arg (*argv);
    std::string targ = arg; // temp. mangled argument
    
    // match long options
    if (arg.size () >= 2 && arg[0] == '-' && arg[1] == '-') {
      targ.erase (0, 2);
      iterator it = long_content.find (targ);
      if (it != long_content.end () )
	argument = it->second;
    }
    // match short options
    else if (arg.size () >= 1 && arg[0] == '-') {
      targ.erase (0, 1);
      iterator it = short_content.find (targ);
      if (it != short_content.end () )
	argument = it->second;
    }
    // try to pass to the last matched argument
    else if (argument) {
      if (!argument->Read (arg) )
	++ errors;
    }
    
    // throw into argument if it does not need arguments (most often bools)
    if (argument && !argument->needs_arg) {
      if (!argument->Read () ) {
	argument = 0;
	++ errors;
      }
    }
    
    if (!argument) {
      std::cout << "Error: Unrecognized argument: " << arg << std::endl;
      ++ errors;
    }
    ++ argv;
  }
  return errors == 0;
}

void ArgumentList::Useage (const std::ostream& os)
{
  for (iterator it = long_content.begin (); it != long_content.end (); ++it)
    {
      std::cout << "  -" << it->second->sname << ", --" << it->second->lname
		<< std::endl << "\t"
		<< it->second->desc << std::endl;
    }
}
