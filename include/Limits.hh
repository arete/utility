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
 *   Numeric limit templates.
 */

#ifndef UTILITY__LIMITS_HH__
#define UTILITY__LIMITS_HH__

namespace Utility
{
  template <class U, class V>
  inline const U limit_min (const U& value, const V& min)
  {
    if (value < min) //__attribute__((unlikely))
      return min;
    else
      return value;
  };
  
  template <class U, class V>
  inline const U limit_max (const U& value, const V& max)
  {
    if (value > max) //__attribute__((unlikely))
      return max;
    else
      return value;
  };
  
  // should only be used when min < max ... !!
  template <class U, class V, class W>
  inline const U limit_min_max (const U& value, const V& min, const W& max)
  {
    // TODO: warn if debug and min > max !!
    if (value < min) //__attribute__((unlikely))
      return min;
    else if (value > max) //__attribute__((unlikely))
      return max;
    else
      return value;
  };
  
} // end namespace utility

#endif // UTILITY__LIMITS_HH__
