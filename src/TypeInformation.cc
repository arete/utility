/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/src/TypeInformation.cc
 * General Sound Manipulation Program is Copyright (C) 2000 - 2003
 *   Valentin Ziegler and Ren� Rebe
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2. A copy of the GNU General
 * Public License can be found at LICENSE.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
 * ABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * --- GSMP-COPYRIGHT-NOTE-END ---
 */

#include "TypeInformation.hh"
#include <cxxabi.h>

using namespace Utility;

std::string Utility::FilteredName (const char* tname)
{
  int status;
  char* ret_c ( abi::__cxa_demangle(tname, 0, 0, &status) );
  std::string ret (ret_c);
  free (ret_c);
  return ret;  
}

std::string Utility::FilteredName (std::string& tname)
{
  return Utility::FilteredName (tname.c_str ());
}
