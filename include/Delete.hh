/*  GSML - General Sound Manipulation Library
 *  Copyright (C) 2000-2002 Ren� Rebe and Valentin Ziegler
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
 *   Object delete function and functor - to help deleting objects e.g.
 *   with STL algorithms.
 */

#ifndef UTILITY__DELETE_HH__
#define UTILITY__DELETE_HH__

namespace Utility
{
  // to delete container of objects via for_each ...
  template <class T>
  class DelFunctor
  {
  public:
    void operator () (T& del_obj) {
      delete del_obj;
      del_obj = 0;
    }
  };
  
  template <class T>
  void DelFunction (T& del_obj) {
    delete del_obj;
    del_obj = 0;
  }
  
} // end namespace utility

#endif // UTILITY__DELETE_HH__
