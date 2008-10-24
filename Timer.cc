/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: utility/src/Timer.cc
 * General Sound Manipulation Program is Copyright (C) 2000 - 2008
 *   Valentin Ziegler and René Rebe
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

/*
 * Alternatively, commercial licensing options are available from the
 * copyright holder ExactCODE GmbH Germany.
 */

#include "Timer.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef __WIN32__
#include <windows.h> / Sleep()
#endif

#include <iostream>

#ifndef __WIN32__

Utility::TickTimer::TickTimer ()
{
  times (&m_times);
}

void Utility::TickTimer::Reset ()
{
  times (&m_times);
}

uint64_t Utility::TickTimer::Delta () const
{
  tms t_times;
  times (&t_times);
  
  return t_times.tms_utime - m_times.tms_utime;
}

uint64_t Utility::TickTimer::Value () const
{
  tms t_times;
  times (&t_times);
  
  return t_times.tms_utime;
}

uint64_t Utility::TickTimer::PerSecond () const
{
  return sysconf (_SC_CLK_TCK);
}

#endif

// ---

Utility::TimebaseTimer::TimebaseTimer ()
{
  start_tick = Value ();
}

void Utility::TimebaseTimer::Reset ()
{
  start_tick = Value ();
}

uint64_t Utility::TimebaseTimer::Delta () const
{
  return Value () - start_tick;
}

uint64_t Utility::TimebaseTimer::Value () const
{
#if defined(__i386__)
  uint64_t x;
  __asm__ __volatile__ (".byte 0x0f, 0x31" : "=A" (x));
  return x;
#elif defined(__x86_64__)
  uint32_t hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
#elif defined(__powerpc64__)
  uint64_t x;
  __asm__ __volatile__ (
			"\tmftb    %0"
			: "=r"(x)
			);
  return x;
#elif defined(__powerpc__) || defined(__ppc__)
  uint32_t hi, lo, tmp;
  __asm__ __volatile__ (
			"0:                  \n"
			"\tmftbu   %0           \n"
			"\tmftb    %1           \n"
			"\tmftbu   %2           \n"
			"\tcmpw    %2,%0        \n"
			"\tbne     0b         \n"
			: "=r"(hi),"=r"(lo),"=r"(tmp)
			);
  return ((uint64_t)hi << 32) | lo;
#elif defined(__sparc_v9__)
#ifdef __LP64__
  uint64_t ticks;
  __asm__ volatile(
		   "rd %%tick, %0\n"
		   : "=r" (ticks) //  : "0" (ticks)
		   );
  return ticks;
#else
  uint32_t hi, lo;
  __asm__ volatile(
		   "rd	%%tick,%%g3\n"
		   "\tor	%%g3,0,%0\n"	// lower bits
		   "\tsrlx	%%g3,32,%%g3\n"
		   "\tor	%%g3,0,%1"	// higher bits 
		   : "=r" (lo), "=r" (hi)
		   );
  return ((uint64_t)hi << 32) | lo;
#endif
#elif define(__mips__)
#error "MIPS timebase support not yet implemented."
  // __asm__ __volatile__ ("dmfc0 %0,$9" : "=r" (ticks));
#elif defined(__ia64__)
#error "IA64 timebase support not yet implemented."
  // __asm__ __volatile__ ("mov %0=ar.itc ;;" : "=rO" (ticks));
#else
  // TODO (at least): ARM, SuperH, then AVR32, Blackfin, Alpha, ...
#error "No CPU timebase read implemented for this architecture, yet!"
#endif
}

uint64_t Utility::TimebaseTimer::PerSecond () const
{
  static uint64_t per_second = 0;
  
  // meassure, not yet very accurate, depends on a exact 1s schedule of the OS
  if (!per_second) {
    uint64_t s1 = Value ();
#ifndef __WIN32_
    sleep (1);
#else
    Sleep (1000);
#endif
    uint64_t s2 = Value ();
    per_second = s2 - s1;
  }
  
  return per_second;
}
