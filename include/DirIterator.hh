/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/include/DirIterator.hh
 * General Sound Manipulation Program is Copyright (C) 2000 - 2003
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

/* Short Description:
 *   A simple directory iterator.
 */

#ifndef UTILITY__DIRITERATOR_HH__
#define UTILITY__DIRITERATOR_HH__

#include <dirent.h>    // used by directory iterator
#include <string>

namespace Utility
{
  
  /* Find a unique filename, "fname + i". The first i can be
     specified.  The fname is overwritten - the used index is
     returned. - A file is unique when there is no file with the
     tested name -> even if the trailing path is not available */
  int FindUniqueName (std::string& fname, const std::string& base,
		      const std::string& ext,
		      int first_tried_index = 0);
  
  /* this is some wrapping around the (ugly) posix directory stuff
     not really an iterator ... */
  class DirList
  {
  public:
    class Iterator
    {
    public:
      Iterator ();
      Iterator (DirList* i_dirlist);
      Iterator (const Iterator& i_other);
      ~Iterator ();
      
      const Iterator& operator++ ();
      const Iterator operator++ (int);
      
      const std::string& operator* ();
      
      bool operator== (const Iterator& other);
      bool operator!= (const Iterator& other);
      
      friend class Utility::DirList;
      
    private:
      
      void Open ();
      void Close ();
      void Next ();
      
      // direct state
      bool m_open;
      bool m_end;
      DirList* m_dirlist;
      std::string m_entry_name;
      
      // indirect state of C API
      DIR* m_internal_dir;
      dirent* m_internal_dir_entry;
  
    };
    
    DirList (const std::string& i_dirmname = "/");
    ~DirList ();
    
#ifdef TODO
    // navigate in the directory tree
    void Down ();
    bool Up (const Iterator& it);
    // navigation alias
    bool Enter (const Iterator& it) {
      return Up (it);
    };
#endif
    
    const Iterator Begin ();
    const Iterator End ();
    
    friend class Iterator;
    
  protected:
    
    std::string m_dirname;
  };
  
} // end namespace utility

#endif // UTILITY__DIRITERATOR_HH__
