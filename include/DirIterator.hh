/*  GSML - General Sound Manipulation Library
 *  Copyright (C) 2000-2002 René Rebe and Valentin Ziegler
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
     tested name -> even if the trailing path is not available
     ... !!?!? ... */
  int FindUniqueName (std::string& fname, const std::string& base,
		      const std::string& ext,
		      int first_tried_index = 0);
  
  /* this is some wrapping arround the (ugly) posix directory stuff
     not really an iterator ... */
  class DirIterator
  {
  public:
    DirIterator ();
    DirIterator (const std::string& i_dir);
    ~DirIterator ();
   
    bool Open (const std::string& n_dirname);
    bool Close ();
    
    const DirIterator& operator++ ();
    const DirIterator& operator++ (int);
    
    bool End ();
    
    const std::string& operator* ();
    
  private:
    void Next ();
    
    const DirIterator& operator= (const DirIterator src);
    
    bool m_open;
    bool m_end;
    std::string m_dirname;
    
    DIR* m_internal_dir;
    dirent* m_internal_dir_entry;
    std::string m_entry_name;
  };
  
} // end namespace utility

#endif // UTILITY__DIRITERATOR_HH__
