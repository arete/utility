/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/src/Timer.cc
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
