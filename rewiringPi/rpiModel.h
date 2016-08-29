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

#ifndef REWIRINGPI_PIMODEL_H
#define REWIRINGPI_PIMODEL_H

/* Pi model types and version numbers
 * Intended for the GPIO program Use at your own risk.
 */

#define	PI_MODEL_A          0
#define	PI_MODEL_B          1
#define	PI_MODEL_AP         2
#define	PI_MODEL_BP         3
#define	PI_MODEL_2          4
#define	PI_ALPHA            5
#define	PI_MODEL_CM         6
#define	PI_MODEL_07         7
#define	PI_MODEL_3          8
#define	PI_MODEL_ZERO       9

#define	PI_VERSION_1        0
#define	PI_VERSION_1_1      1
#define	PI_VERSION_1_2      2
#define	PI_VERSION_2        3

#define	PI_MAKER_SONY       0
#define	PI_MAKER_EGOMAN     1
#define	PI_MAKER_MBEST      2
#define	PI_MAKER_UNKNOWN    3

/**********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

int setPiModel2(int value);
int isPiModel2(void);


#ifdef __cplusplus
}
#endif

/**********************************************************************/

#endif // REWIRINGPI_PIMODEL_H
