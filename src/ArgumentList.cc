/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/src/ArgumentList.cc
 * General Sound Manipulation Program is Copyright (C) 2000 - 2004
 *   Valentin Ziegler and Ren� Rebe
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2. A copy of the GNU General
 * Public License can be found in the file LICENSE.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
 * ABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * --- GSMP-COPYRIGHT-NOTE-END ---
 */

#include <sstream>

#include "ArgumentList.hh"
#include "template/ArgumentList.tcc"

using namespace Utility;

BasicArgument::BasicArgument (const std::string& i_sname, const std::string& i_lname,
			      const std::string& i_desc,  int i_min_count, int i_max_count,
			      bool i_fragmented, bool i_list)
  : count (0), pass_count (0)
{
  sname = i_sname;
  lname = i_lname;
  desc = i_desc;

  min_count  = i_min_count;
  max_count = i_max_count;
  fragmented = i_fragmented;
  list = i_list;
  
  // sanity check
  if (min_count > max_count)
    max_count = min_count;
  
  no_arg = max_count == 0;
}

BasicArgument::~BasicArgument ()
{
}

bool BasicArgument::Probe () {
  return count < max_count;
}

bool BasicArgument::Interrupt () {
  if (!no_arg && pass_count == 0) {
    std::cout << "Error: No parameter for argument " << lname << " specified!"
	      << std::endl;
    return false;
  }
  else if (!fragmented && count < min_count) {
    std::cout << "Error: No fragmentation for argument " << lname << " allowed!"
	      << std::endl
	      << "       At least " << min_count << " parameter required!" << std::endl;
    pass_count = 0;
    return false;
  }
  pass_count = 0;
  return true;
}
  
bool BasicArgument::Finalize () {
  if (count < min_count) {
    std::cout << "Error: Too few parameter for argument " << lname
	      << ", at least " << min_count << " required!" << std::endl;
    return false;
  }
  return true;
}

ArgumentList::ArgumentList (bool i_residual)
{
  residual = i_residual;
}
  
ArgumentList::~ArgumentList() {
  
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
  bool residual_gathering = false;
  
  // parse all arguments
  while (argv != argv_end) {
    std::string arg (*argv);
    std::string targ = arg; // temp. mangled argument

    BasicArgument* new_argument = 0;
    
    // match long options
    if (arg.size () >= 2 && arg[0] == '-' && arg[1] == '-') {
      targ.erase (0, 2);
      iterator it = long_content.find (targ);
      if (it != long_content.end () ) {
	new_argument = it->second;
      }
    }
    // match short options
    else if (arg.size () >= 1 && arg[0] == '-') {
      targ.erase (0, 1);
      iterator it = short_content.find (targ);
      if (it != short_content.end () )
	new_argument = it->second;
    }
    
    if (new_argument) {
      if (argument && !argument->Interrupt () )
	++ errors;
      argument = new_argument;
    }
    // try to parse via the last matched argument
    // start residual gathering if not parseable
    else if (argument) {
      if (residual && !argument->Probe()) {
	argument = 0;
	residual_gathering = true;
      }
      else {
	if (!argument->Read (arg) )
	  ++ errors;
      }
    }
    
    // immediately throw into argument if it does not need parameters
    // (most often bools)
    if (argument && argument->no_arg) {
      if (!argument->Read () ) {
	argument = 0;
	++ errors;
      }
    }
    
    // warning if in residual_gathering mode
    if (residual_gathering) {
      residuals.push_back (arg);
      if (argument)
	std::cout << "Warning: Residual parameter " << arg
		  << " matches an argument" <<  std::endl;
    }
    else
      if (!argument) {
	std::cout << "Error: Unrecognized argument: " << arg << std::endl;
	++ errors;
      }
    
    ++ argv;
  }
  // interrupt the last parsed argument
  if (argument && !argument->Interrupt () )
    ++ errors;
  
  // final checks
  for (iterator it = long_content.begin (); it != long_content.end (); ++it) {
    if (!it->second->Finalize())
      ++ errors;
  }
  
  return errors == 0;
}

const std::vector<std::string>& ArgumentList::Residuals () const
{
  return residuals;
}

void ArgumentList::Usage (const std::ostream& os) const
{
  for (const_iterator it = long_content.begin (); it != long_content.end (); ++it)
    if (it->second->list)
      std::cout << "  -" << it->second->sname << ", --" << it->second->lname
		<< std::endl << "\t"
		<< it->second->desc << std::endl;
}
