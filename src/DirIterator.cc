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

#include "DirIterator.hh"

#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <sstream> // for stringstream

int Utility::FindUniqueName (std::string& fname, const std::string& base,
			     const std::string& ext, int first_tried_index)
{
  std::stringstream t_fname;
  
  int t_index = first_tried_index;
  bool t_found = false;
  while (!t_found)
    {
      t_fname.str ("");
      t_fname << base << t_index << ext;
      std::cout << "testing: (statting): " << t_fname.str () << std::endl;
      struct stat t_stat;
      if (stat (t_fname.str().c_str (), &t_stat) < 0)
	t_found = true;
      else
	++ t_index;
    }
  fname = t_fname.str ();
  return t_index;
}


Utility::DirIterator::DirIterator (const std::string& i_dir)
{
  m_open = false;
  m_end = false;
  Open (i_dir);
}

Utility::DirIterator::~DirIterator ()
{
  if (m_open)
    Close ();
}

bool Utility::DirIterator::Open (const std::string& n_dirname)
{
  if (m_open)
    return false;
  
  m_dirname = n_dirname;
  
  m_internal_dir = opendir (m_dirname.c_str () );
  if (m_internal_dir == 0)
    return false;
  
  m_open = true;
  m_end = false;
  
  Next ();
  
  return true;
}

bool Utility::DirIterator::Close ()
{
  if (!m_open)
    return false;
  
  if (closedir (m_internal_dir) < 0)
    return false;
  
  m_open = false;
  return true;
}

const Utility::DirIterator& Utility::DirIterator::operator++ ()
{
  Next ();
}

const Utility::DirIterator& Utility::DirIterator::operator++ (int)
{
  Next ();
}

bool Utility::DirIterator::End ()
{
  return m_end;
}

const std::string& Utility::DirIterator::operator* ()
{
  return m_entry_name;
}


void Utility::DirIterator::Next ()
{
  if (!m_open)
    return;
  
  m_internal_dir_entry = readdir (m_internal_dir);
  if (m_internal_dir_entry == 0) {
    m_entry_name = "";
    m_end = true;
    return;
  }
  
  m_entry_name = m_internal_dir_entry->d_name;
}
