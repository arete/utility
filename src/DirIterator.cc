/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/src/DirIterator.cc
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

// ---

Utility::DirList::Iterator::Iterator ()
{
  m_open = false;
  m_end = false;
  m_dirlist = 0;
}

Utility::DirList::Iterator::Iterator (DirList* i_dirlist)
{
  m_open = false;
  m_end = false;
  m_dirlist = i_dirlist;
  m_entry_name = "";
  
  Open ();
}

Utility::DirList::Iterator::Iterator (const Iterator& i_other)
{
  m_open = false;
  m_end = i_other.m_end;
  m_dirlist = i_other.m_dirlist;
  m_entry_name = i_other.m_entry_name;
}

Utility::DirList::Iterator::~Iterator ()
{
  if (m_open)
    Close ();
}

const Utility::DirList::Iterator& Utility::DirList::Iterator::operator++ ()
{
  Next ();
  return *this;
}

const Utility::DirList::Iterator Utility::DirList::Iterator::operator++ (int)
{
  Iterator it (*this);
  Next ();
  return it;
}

void Utility::DirList::Iterator::Open ()
{
  if (m_open)
    return;
  
  // special case: we have a dirname but are not yet open (e.g.
  // after a operator= and so want to search to the correct entry
  std::string t_entry_name = m_entry_name;
  
  m_internal_dir = opendir (m_dirlist->m_dirname.c_str () );
  if (m_internal_dir == 0) {
    m_end = true;
  }
  else {
    m_open = true;
    m_end = false;
    
    Next ();
    
    // special case (also see above) we want to search an entry
    if (t_entry_name != "") {
      while (m_entry_name != t_entry_name && !m_end)
	Next ();
    }
  }
}

void Utility::DirList::Iterator::Close ()
{
  if (!m_open)
    return;
  
  //if (closedir (m_internal_dir) < 0)
  
  m_open = false;
}

const std::string& Utility::DirList::Iterator::operator* ()
{
  return m_entry_name;
}

bool Utility::DirList::Iterator::operator== (const DirList::Iterator& other)
{
  return (m_dirlist == other.m_dirlist &&
	  m_end == other.m_end &&
	  m_entry_name == other.m_entry_name);
}

bool Utility::DirList::Iterator::operator!= (const DirList::Iterator& other)
{
  return (m_dirlist != other.m_dirlist ||
	  m_end != other.m_end ||
	  m_entry_name != other.m_entry_name);
}

void Utility::DirList::Iterator::Next ()
{
  if (!m_open) {
    m_end = true;
    return;
  }
  
  m_internal_dir_entry = readdir (m_internal_dir);
  if (m_internal_dir_entry == 0) {
    m_entry_name = "";
    m_end = true;
    return;
  }
  
  // skip . and .. - any dir has those and apps normally do not need them
  m_entry_name = m_internal_dir_entry->d_name;
  if (m_entry_name == "." || m_entry_name == "..")
    Next ();
}

// ---

Utility::DirList::DirList (const std::string& i_dirname)
{
  m_dirname = i_dirname;
}

Utility::DirList::~DirList ()
{
  
}

const Utility::DirList::Iterator Utility::DirList::Begin ()
{
  // create a virgin Iterator
  Iterator it (this);
  return it;
}

const Utility::DirList::Iterator Utility::DirList::End ()
{
  // create a Iterator with end flag set
  Iterator it (this);
  it.m_end = true;
  it.m_entry_name = "";
  return it;
}
