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

#include "rpiModel.h"
#include "types.h"

/* Data for use with the boardId functions.
 * The order of entries here to correspond with the PI_MODEL_X
 * and PI_VERSION_X defines in rpiModel.h
 * Only intended for the gpio command - use at your own risk!
 */

static int piModel2 = false;

static const char *piModelNames[16] = {
	"Model A",          // 0
	"Model B",          // 1
	"Model A+",         // 2
	"Model B+",         // 3
	"Pi 2",             // 4
	"Alpha",            // 5
	"CM",               // 6
	"Unknown07",        // 7
	"Pi 3",             // 8
	"Pi Zero",          // 9
	"Unknown10",        // 10
	"Unknown11",        // 11
	"Unknown12",        // 12
	"Unknown13",        // 13
	"Unknown14",        // 14
	"Unknown15",        // 15
};

static const char *piRevisionNames[16] = {
	"00",
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
};

static const char *piMakerNames[16] = {
	"Sony",             // 0
	"Egoman",           // 1
	"Embest",           // 2
	"Unknown",          // 3
	"Embest",           // 4
	"Unknown05",        // 5
	"Unknown06",        // 6
	"Unknown07",        // 7
	"Unknown08",        // 8
	"Unknown09",        // 9
	"Unknown10",        // 10
	"Unknown11",        // 11
	"Unknown12",        // 12
	"Unknown13",        // 13
	"Unknown14",        // 14
	"Unknown15",        // 15
};

static const int piMemorySize[8] = {
	256,                // 0
	512,                // 1
	1024,               // 2
	0,                  // 3
	0,                  // 4
	0,                  // 5
	0,                  // 6
	0,                  // 7
};

/**********************************************************************/

int setPiModel2(int value)
{
	return piModel2 = !!value;
}

int isPiModel2()
{
	return piModel2;
}
