/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/include/Storage.hh
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

#ifndef UTILITY__STORAGE_HH__
#define UTILITY__STORAGE_HH__

#include <string>
#include <vector>
#include <map>
#include <iostream>

class BasicStorage {
 public:
  
  virtual ~BasicStorage () {};
  
  virtual std::istream& Read (std::istream& is) = 0;
  virtual std::ostream& Write (std::ostream& os) = 0;

  std::string name;
};

template <typename T> 
class Storage : public BasicStorage {
 
 public:
  
  Storage (const std::string& i_name) {
    name = i_name;
  }

  Storage (const std::string& i_name, const T& i_value) {
    name = i_name;
    value = i_value;
  }
  
  virtual std::istream& Read (std::istream& is) {
    // TODO: error handling
    is >> value;
    return is;
  }
  
  virtual std::ostream& Write (std::ostream& os) {
    os << value;
    return os;
  }
  
  T value;
};

// some bool specialisations ...
template <> std::istream& Storage<bool>::Read (std::istream& is);

class Serializer {

 public:

  void Add (BasicStorage* storage);
  
  std::istream& Read (std::istream& is, bool verbose = true);

  template <typename T>
  T Get (const std::string& name) {
    iterator it = content.find (name);
    
    if (it == content.end () ) {
      std::cout << "Warning: Storage " << name << " not available!"
		<< std::endl;
      return T ();
    }
    
    Storage<T>* s;
    s = dynamic_cast<Storage<T>*> (it->second);
    if (s)
      return s->value;
    else {
      std::cout << "Warning: Type mismatch for storage " << name << "!"
		<< std::endl;
      return T ();
    }
  }
  
 private:
  typedef std::map<std::string, BasicStorage*> container;
  typedef container::iterator iterator;
  
  container content;
};

#include "template/Storage.tcc"

#endif // UTILITY__STORAGE__
