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

#include "ThreadHelper.hh"
#include "Limits.hh"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>        // Linux/POSIX scheduler / realtime scheduling ...
#include <sys/resource.h> // set_priority

#include <iostream>

void Utility::StopInDebugger ()
{
  pid_t m_pid = getpid ();
  kill (m_pid, SIGTRAP);
}

bool Utility::EnableRealtimeScheduling ()
{
  int t_pri = sched_get_priority_min (SCHED_FIFO);
  std::cout << "ThreadHelper:: priority_min: " << t_pri << std::endl;
  
  if (t_pri >= 0) {
    sched_param t_params;
    t_params.sched_priority = t_pri;
    
    int error = sched_setscheduler (0, SCHED_FIFO, &t_params);
    
    if (error < 0)
      std::cout << "ThreadHelper:: WARNING POSIX realtime-scheduling "
		<< "not available (not root?)!" << std::endl;
    return error >= 0;
  }
  return false;
}

void Utility::USleep (int delay)
{
  /* From the nanosleep man-page:
   * 
   * If  the process is scheduled under a real-time policy like
   * SCHED_FIFO or SCHED_RR, then pauses of up to 2 ms will  be
   * performed as busy waits with microsecond precision.
   */
  
  /* Temp disable the sched_yield feature.
   * Ee call it too often with such a little value ... */
  delay = limit_min (delay, 5000); 
  if (delay < 500)
    sched_yield ();
  else
    usleep (delay);
}

bool Utility::SetPriority (int priority)
{
  return setpriority (PRIO_PROCESS, 0, priority) >= 0;
}
