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
 *   Some basic timer classes.
 */

#ifndef UTILITY__TIMER_HH__
#define UTILITY__TIMER_HH__

#include <sys/time.h>  // used by Timer
#include <sys/times.h> // used by TickTimer

#include <string>

namespace Utility
{
  class Timer
  {
  public:
    Timer ();
    
    void Reset ();
    long long Delta ();
    long long PerSecond ();
    
  private:
    timeval m_start;
  };
  
  class TickTimer
  {
  public:
    TickTimer ();
    
    void Reset ();
    long long Delta ();
    long long PerSecond ();
    
  private:
    struct tms m_times;
  };

  template <typename T>
  class AutoTimer
  {
  public:
    AutoTimer (const std::string& i_text) {
      m_text = i_text;
    }
    ~AutoTimer () {
      std::cout << ">> AutoTimer: \"" << m_text
		<< "\" took: " << m_timer.Delta () << " us" << std::endl;
    }
    
    long long Delta () {
      return m_timer.Delta ();
    }
    long long PerSecond () {
      return m_timer.PerSecond ();
    }
    
  private:
    T m_timer;
    std::string m_text;
  };
  
} // end namespace utility

#endif // UTILITY__TIMER_HH__
