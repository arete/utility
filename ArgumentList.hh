/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/include/ArgumentList.hh
 * General Sound Manipulation Program is Copyright (C) 2000 - 2004
 *   Valentin Ziegler and Ren頒ebe
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

/* Short Description:
 *   Object oriented argument list parsing.
 */

#ifndef UTILITY__ARGUMENTLIST_HH__
#define UTILITY__ARGUMENTLIST_HH__

#include <iostream>

#include <map>
#include <vector>
#include <string>

#include <sstream>

namespace Utility
{
  class BasicArgument {
  public:
  
    BasicArgument (const std::string& i_sname, const std::string& i_lname,
		   const std::string& i_desc,
		   int i_min_count, int i_max_count,
		   bool i_fragmented, bool i_reset);
  
    virtual ~BasicArgument ();
    
    void SetReset (bool i_reset) { reset=i_reset; }
    void SetList (bool i_list) { list=i_list; }
  
    virtual bool Probe ();
    virtual void Start () = 0;
    
    virtual bool Read () = 0;
    virtual bool Read (const std::string& arg) = 0;
    
    virtual bool Interrupt ();
    virtual bool Finalize ();
    
    std::string sname;
    std::string lname;
    std::string desc;
  
    int min_count, max_count;
    bool no_arg;
    bool fragmented; // fragment across several arguments
    bool reset; // reset (clear) for each new argument
    bool list; // list in the Usage
    
    int count, pass_count;
    
  protected:
    virtual bool InterruptImpl () {
      return true;
    }
  };

  template <typename T>
  class Argument : public BasicArgument {
 
  public:
  
    Argument (const std::string& i_sname, const std::string& i_lname,
	      const std::string& i_desc,
	      int i_min_count = 0, int i_max_count = 0,
	      bool i_fragmented = false, bool i_reset = false)
      : BasicArgument (i_sname, i_lname, i_desc, i_min_count, i_max_count,
		       i_fragmented, i_reset),
	callback (0)
    {
      // if bool (or other special option) always use exaclty one value
      if (no_arg)
	values.push_back (T () );
    }

    Argument (const std::string& i_sname, const std::string& i_lname,
	      const std::string& i_desc, 
	      const T& i_value, int i_min_count = 0, int i_max_count = 0,
	      bool i_fragmented = false, bool i_reset = false)
      : BasicArgument (i_sname, i_lname, i_desc, i_min_count, i_max_count,
		       i_fragmented, i_reset),
	callback (0)
    {
      values.push_back (i_value);
      // if bool (or other special option) always containing one value
      if (no_arg)
	values.push_back (T () );
    }
  
    
    void Start () {
      // reset requested?
      if (!no_arg && reset) {
	values.clear ();
	count = 0;
      }
    }
    
    bool Read () {
      std::cerr << "Error: Argument " << lname << " needs an parameter!" << std::endl;
      return false;
    }
  
    bool Read (const std::string& arg) {
      
      if (count >= max_count) {
	std::cerr << "Error: Too many parameter for argument " << lname
		  << ", only " << max_count << " allowed!" << std::endl;
	return false;
      }
      
      T value = ReadImpl (arg);
      
      // special case if default was supplied -> overwrite the first ...
      if (count == 0 & values.size () > 0)
	values[0] = value;
      else
	values.push_back (value);
    
      ++count; ++pass_count;
    
      return true;
    }
  
    T Get (unsigned int i = 0) const {
      if (values.size () > i) {
	return values [i];
      }
      else
	std::cerr << "Error: There is no parameter: " << i
		  << " present for argument " << this->lname << std::endl;
      return T ();
    }
    
    int Size () const { return values.size(); }
    
    void Bind (bool (*function)(const Argument<T>&)) {
      callback = function;
    }
    
    const std::vector<T>& Values () const { return values; }
    
  private:
    
    T ReadImpl (const std::string& arg)
    {
      std::stringstream stream (arg);
      // TODO: error handling
      T value;
      stream >> value;
      return value;
    }
    
    virtual bool InterruptImpl () {
      if (callback)
	return callback(*this);
      return true;
    }
    
    std::vector<T> values;
    bool (*callback)(const Argument<T>&);
  };

  // some bool specialisations ...
  template <> bool Argument<bool>::Read ();
  template <> bool Argument<bool>::Read (const std::string& arg);
  
  template <> std::string Argument<std::string>::ReadImpl (const std::string& arg);

  class ArgumentList {

  public:
  
    // inresidual mode the last stray parameters are gathered
    ArgumentList (bool i_residual = false);
    ~ArgumentList ();

    // register a to be parsed argument
    void Add (BasicArgument* arg);
    // parse options specified
    bool Read (int argc, char** argv);
  
    // return the vector of gathered residuals (if residual parsing enabled)
    const std::vector<std::string>& Residuals () const;

    // printout the usual usage list, generated from the registered arguments
    void Usage (std::ostream& os) const;
    
  private:
    typedef std::map<std::string, BasicArgument*> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;
  
    container short_content;
    container long_content;
  
    std::vector<std::string> residuals;
  
    bool residual;
  };

} // namespace Utility

#include "template/ArgumentList.tcc"

#endif // UTILITY__ARGUMENTLIST_HH__
