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

#include "Timer.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

Utility::Timer::Timer ()
{
  gettimeofday (&m_start, NULL);
}

void Utility::Timer::Reset ()
{
  gettimeofday (&m_start, NULL);
}

long long Utility::Timer::Delta ()
{
  long long ctime;
  
  timeval t_time;
  gettimeofday (&t_time, NULL);
  
  ctime = (t_time.tv_sec * 1000000) + t_time.tv_usec;
  ctime -= ( (m_start.tv_sec * 1000000) + m_start.tv_usec);
  
  return ctime;
}

long long Utility::Timer::PerSecond ()
{
  return 1000000;
}

Utility::TickTimer::TickTimer ()
{
  times (&m_times);
}

void Utility::TickTimer::Reset ()
{
  times (&m_times);
}

long long Utility::TickTimer::Delta ()
{
  tms t_times;
  times (&t_times);
  
  return t_times.tms_utime - m_times.tms_utime;
}

long long Utility::TickTimer::PerSecond ()
{
  return sysconf (_SC_CLK_TCK);
}
