/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/include/template/ArgumentList.tcc
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

#include "Compiler.hh"

#if GCC_VERSION < 30400 || __INCLUDE_LEVEL__ == 1

#ifndef UTILITY__ARGUMENTLIST_TCC__
#define UTILITY__ARGUMENTLIST_TCC__

namespace Utility {
  
  template <> bool Argument<bool>::Read () {
    // the ctor garuantees we have one value!
    values[0] = true;
  ++ count;
  return true;
  }
  
  template <> bool Argument<bool>::Read (const std::string& arg) {
    std::cout << "Error: No parameter allowed for boolean values!" << std::endl;
    return false;
  }
  
} // namespace Utility

#endif // UTILITY__ARGUMENTLIST_TCC__

#endif

