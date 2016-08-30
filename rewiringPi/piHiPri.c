/***********************************************************************
 *    Copyright (c) 2016 Craig Robbins
 *    Copyright (c) 2012-2016 Gordon Henderson
 ***********************************************************************
 *    This file is part of rewiringPi. rewiringPi is a fork of
 *    wiringPi (https://projects.drogon.net/raspberry-pi/wiringpi/)
 *
 *    rewiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    rewiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with rewiringPi.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/
#include <sched.h>
#include <string.h>

#include "rwPi.h"

/*!
 * Attempt to set a high priority scheduling for the running program.
 */
int piHiPri (int pri)
{
  struct sched_param sched;

  memset(&sched, 0, sizeof(sched));

  if (pri > sched_get_priority_max(SCHED_RR))
    sched.sched_priority = sched_get_priority_max(SCHED_RR);
  else
    sched.sched_priority = pri;

  return sched_setscheduler(0, SCHED_RR, &sched) ;
}
