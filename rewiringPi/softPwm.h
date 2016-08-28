/***********************************************************************
 *    Copyright (c) 2016 Craig Robbins
 *    Copyright (c) 2012-2016 Gordon Henderson
 ***********************************************************************
 *    This file is part of rewiringPi
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

#ifdef __cplusplus
extern "C" {
#endif

extern int  softPwmCreate (int pin, int value, int range);
extern void softPwmWrite  (int pin, int value);
extern void softPwmStop   (int pin);

#ifdef __cplusplus
}
#endif
