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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rpiModel.h"
#include "types.h"
#include "globals.h"

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

int isPiModel2(void)
{
	return piModel2;
}

const char *getPiModelName(unsigned model_id)
{
	return piModelNames[model_id & 0x0f];
}

const char *getPiRevisionName(unsigned rev_id)
{
	return piRevisionNames[rev_id & 0x0f];
}

const char *getPiMakerName(unsigned maker_id)
{
	return piMakerNames[maker_id & 0x0f];
}

int getPiMemorySize(enum MemorySizeId mem_sz_id)
{
	return piMemorySize[mem_sz_id & 0x07];
}

/**********************************************************************/

/*
 * piBoardRev:
 *	Return a number representing the hardware revision of the board.
 *	This is not strictly the board revision but is used to check the
 *	layout of the GPIO connector - and there are 2 types that we are
 *	really interested in here. The very earliest Pi's and the
 *	ones that came after that which switched some pins ....
 *
 *	Revision 1 really means the early Model A and B's.
 *	Revision 2 is everything else - it covers the B, B+ and CM.
 *		... and the Pi 2 - which is a B+ ++  ...
 *		... and the Pi 0 - which is an A+ ...
 *
 *	The main difference between the revision 1 and 2 system that I use here
 *	is the mapping of the GPIO pins. From revision 2, the Pi Foundation changed
 *	3 GPIO pins on the (original) 26-way header - BCM_GPIO 22 was dropped and
 *	replaced with 27, and 0 + 1 - I2C bus 0 was changed to 2 + 3; I2C bus 1.
 *
 *********************************************************************************
 */

static void piBoardRevOops(const char *why)
{
	fprintf(stderr, "piBoardRev: Unable to determine board revision from /proc/cpuinfo\n");
	fprintf(stderr, " -> %s\n", why);
	fprintf(stderr, " ->  You may want to check:\n");
	fprintf(stderr, " ->  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=184410#p184410\n");
	exit(EXIT_FAILURE);
}

int piBoardRev(void)
{
	FILE *cpuFd;
	char line[120];	// TODO: fix all possible problems with this fixed buffer size
	char *c;
	static int boardRev = -1;

	if (boardRev != -1)	// No point checking twice
		return boardRev;

	if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL)
		piBoardRevOops("Unable to open /proc/cpuinfo");

// Start by looking for the Architecture to make sure we're really running
//	on a Pi. I'm getting fed-up with people whinging at me because
//	they can't get it to work on weirdFruitPi boards...

	while (fgets(line, 120, cpuFd) != NULL)
		if (strncmp(line, "Hardware", 8) == 0)
			break;

	// TODO: If /proc/hardware is an empty stream (hack attempt?) then "line" here
	// is not initialised and possibly an attack vector if there just happens
	// to be no '\0' in the line array
	if (strncmp(line, "Hardware", 8) != 0)
		piBoardRevOops("No hardware line");

	if (wiringPiDebug)
		printf("piboardRev: Hardware: %s\n", line);

// See if it's BCM2708 or BCM2709

	if (strstr(line, "BCM2709") != NULL) {	// Pi v2 - no point doing anything more at this point
		piModel2 = true;
		fclose(cpuFd);
		return boardRev = 2;
	} else if (strstr(line, "BCM2708") == NULL) {
		fprintf(stderr, "Unable to determine hardware version. I see: %s,\n", line);
		fprintf(stderr, " - expecting BCM2708 or BCM2709.\n");
		fprintf(stderr, "If this is a genuine Raspberry Pi then please report this\n");
		fprintf(stderr, "to projects@drogon.net. If this is not a Raspberry Pi then you\n");
		fprintf(stderr, "are on your own as wiringPi is designed to support the\n");
		fprintf(stderr, "Raspberry Pi ONLY.\n");
		exit(EXIT_FAILURE);
	}

// Now do the rest of it as before - we just need to see if it's an older
//	Rev 1 as anything else is rev 2.

// Isolate the Revision line

	rewind(cpuFd);
	while (fgets(line, 120, cpuFd) != NULL)
		if (strncmp(line, "Revision", 8) == 0)
			break;

	fclose(cpuFd);

	if (strncmp(line, "Revision", 8) != 0)
		piBoardRevOops("No \"Revision\" line");

// Chomp trailing CR/NL

	// TODO: Change this to use isspace() and probably make it a function
	for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
		*c = 0;

	if (wiringPiDebug)
		printf("piboardRev: Revision string: %s\n", line);

// Scan to the first character of the revision number
// i.e. the character following the colon
	c = strchr(line, ':');
	if (!c)
		piBoardRevOops("Bogus \"Revision\" line (no colon)");
	c++;

//  TODO: DELETE OLD CODE
//	for (c = line; *c; ++c)
//		if (*c == ':')
//			break;

//	if (*c != ':')
//		piBoardRevOops("Bogus \"Revision\" line (no colon)");
//	++c;

// Chomp spaces

	while (isspace(*c))
		++c;

	if (!isxdigit(*c))
		piBoardRevOops("Bogus \"Revision\" line (no hex digit at start of revision)");

// Make sure its long enough

	if (strlen(c) < 4)
		piBoardRevOops("Bogus revision line (too small)");

// Isolate  last 4 characters:

	c = c + strlen(c) - 4;

	if (wiringPiDebug)
		printf("piboardRev: last4Chars are: \"%s\"\n", c);

	if ((strcmp(c, "0002") == 0) || (strcmp(c, "0003") == 0))
		boardRev = 1;
	else
		boardRev = 2;	// Covers everything else from the B revision 2 to the B+, the Pi v2 and CM's.

	if (wiringPiDebug)
		printf("piBoardRev: Returning revision: %d\n", boardRev);

	return boardRev;
}


/*
 * piBoardId:
 *	Return the real details of the board we have.
 *
 *	This is undocumented and really only intended for the GPIO command.
 *	Use at your own risk!
 *
 *	Seems there are some boards with 0000 in them (mistake in manufacture)
 *	So the distinction between boards that I can see is:
 *
 *		0000 - Error
 *		0001 - Not used
 *
 *      Original Pi boards:
 *              0002 - Model B,  Rev 1,   256MB, Egoman
 *              0003 - Model B,  Rev 1.1, 256MB, Egoman, Fuses/D14 removed.
 *
 *      Newer Pi's with remapped GPIO:
 *              0004 - Model B,  Rev 1.2, 256MB, Sony
 *              0005 - Model B,  Rev 1.2, 256MB, Egomnn
 *              0006 - Model B,  Rev 1.2, 256MB, Egoman
 *
 *              0007 - Model A,  Rev 1.2, 256MB, Egoman
 *              0008 - Model A,  Rev 1.2, 256MB, Sony
 *              0009 - Model A,  Rev 1.2, 256MB, Egoman
 *
 *              000d - Model B,  Rev 1.2, 512MB, Egoman (Red Pi, Blue Pi?)
 *              000e - Model B,  Rev 1.2, 512MB, Sony
 *              000f - Model B,  Rev 1.2, 512MB, Egoman
 *
 *              0010 - Model B+, Rev 1.2, 512MB, Sony
 *              0013 - Model B+  Rev 1.2, 512MB, Embest
 *              0016 - Model B+  Rev 1.2, 512MB, Sony
 *              0019 - Model B+  Rev 1.2, 512MB, Egoman
 *
 *              0011 - Pi CM,    Rev 1.1, 512MB, Sony
 *              0014 - Pi CM,    Rev 1.1, 512MB, Embest
 *              0017 - Pi CM,    Rev 1.1, 512MB, Sony
 *              001a - Pi CM,    Rev 1.1, 512MB, Egoman
 *
 *              0012 - Model A+  Rev 1.1, 256MB, Sony
 *              0015 - Model A+  Rev 1.1, 256MB, Embest
 *              0018 - Model A+  Rev 1.1, 256MB, Sony
 *              001b - Model A+  Rev 1.1, 256MB, Egoman
 *
 *      A small thorn is the olde style overvolting - that will add in
 *              1000000
 *
 *      The Pi compute module has an revision of 0011 or 0014 - since we only
 *      check the last digit, then it's 1, therefore it'll default to not 2 or
 *      3 for a Rev 1, so will appear as a Rev 2. This is fine for the most part, but
 *      we'll properly detect the Compute Module later and adjust accordingly.
 *
 * And then things changed with the introduction of the v2...
 *
 * For Pi v2 and subsequent models - e.g. the Zero:
 *
 *   [USER:8] [NEW:1] [MEMSIZE:3] [MANUFACTURER:4] [PROCESSOR:4] [TYPE:8] [REV:4]
 *   NEW          23: will be 1 for the new scheme, 0 for the old scheme
 *   MEMSIZE      20: 0=256M 1=512M 2=1G
 *   MANUFACTURER 16: 0=SONY 1=EGOMAN 2=EMBEST
 *   PROCESSOR    12: 0=2835 1=2836
 *   TYPE         04: 0=MODELA 1=MODELB 2=MODELA+ 3=MODELB+ 4=Pi2 MODEL B 5=ALPHA 6=CM
 *   REV          00: 0=REV0 1=REV1 2=REV2
 *********************************************************************************
 */

void piBoardId(int *model, int *rev, int *mem, int *maker, int *warranty)
{
	FILE *cpuFd;
	char line[120];
	char *c;
	unsigned int revision;
	int bRev, bType, bProc, bMfg, bMem, bWarranty;

	// TODO: REFACTOR (this is all copy/paste from piBoardRev())
	// REFACTOR BEGIN

//	Will deal with the properly later on - for now, lets just get it going...
//  unsigned int modelNum;

	(void)piBoardRev();	// Call this first to make sure all's OK. Don't care about the result.

	if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL)
		piBoardRevOops("Unable to open /proc/cpuinfo");

	while (fgets(line, 120, cpuFd) != NULL)
		if (strncmp(line, "Revision", 8) == 0)
			break;

	fclose(cpuFd);

	if (strncmp(line, "Revision", 8) != 0)
		piBoardRevOops("No \"Revision\" line");

// Chomp trailing CR/NL

	for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
		*c = 0;

	if (wiringPiDebug)
		printf("piBoardId: Revision string: %s\n", line);

// Need to work out if it's using the new or old encoding scheme:

// Scan to the first character of the revision number

	for (c = line; *c; ++c)
		if (*c == ':')
			break;

	if (*c != ':')
		piBoardRevOops("Bogus \"Revision\" line (no colon)");

// Chomp spaces

	++c;
	while (isspace(*c))
		++c;

	if (!isxdigit(*c))
		piBoardRevOops("Bogus \"Revision\" line (no hex digit at start of revision)");

// TODO:
// REFACTOR END

	// FIXME: check for error
	revision = (unsigned int)strtol(c, NULL, 16);  // Hex number with no leading 0x

// Check for new way:

	if ((revision & (1 << 23)) != 0) {	// New way
		if (wiringPiDebug)
			printf("piBoardId: New Way: revision is: 0x%08X\n", revision);

		bRev      = (revision & (0x0F <<  0)) >>  0;
		bType     = (revision & (0xFF <<  4)) >>  4;
		bProc     = (revision & (0x0F << 12)) >> 12;	// Not used for now.
		bMfg      = (revision & (0x0F << 16)) >> 16;
		bMem      = (revision & (0x07 << 20)) >> 20;
		bWarranty = (revision & (0x03 << 24)) != 0;

		*model    = bType;
		*rev      = bRev;
		*mem      = bMem;
		*maker    = bMfg ;
		*warranty = bWarranty;

		if (wiringPiDebug)
			printf("piboardId: rev: %d, type: %d, proc: %d, mfg: %d, mem: %d, warranty: %d\n",
				   bRev, bType, bProc, bMfg, bMem, bWarranty);
	} else {				// Old way
		if (wiringPiDebug)
			printf("piBoardId: Old Way: revision is: %s\n", c);

		if (!isdigit(*c))
			piBoardRevOops("Bogus \"Revision\" line (no digit at start of revision)");

// Make sure its long enough

		if (strlen(c) < 4)
			piBoardRevOops("Bogus \"Revision\" line (not long enough)");

// If longer than 4, we'll assume it's been overvolted

		*warranty = strlen(c) > 4;

// Extract last 4 characters:

		c = c + strlen(c) - 4;

// Fill out the replys as appropriate

		if (strcmp(c, "0002") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_1;
			*mem = 0;
			*maker = PI_MAKER_EGOMAN;
		} else if (strcmp(c, "0003") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_1_1;
			*mem = 0;
			*maker = PI_MAKER_EGOMAN;
		} else if (strcmp(c, "0004") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_2 ;
			*mem = 0;
			*maker = PI_MAKER_SONY;
		} else if (strcmp(c, "0005") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_2;
			*mem = 0;
			*maker = PI_MAKER_UNKNOWN;
		} else if (strcmp(c, "0006") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_2;
			*mem = 0;
			*maker = PI_MAKER_EGOMAN;
		} else if (strcmp(c, "0007") == 0) {
			*model = PI_MODEL_A;
			*rev = PI_VERSION_2;
			*mem = 0;
			*maker = PI_MAKER_EGOMAN;
		} else if (strcmp(c, "0008") == 0) {
			*model = PI_MODEL_A ;
			*rev = PI_VERSION_2  ;
			*mem = 0;
			*maker = PI_MAKER_SONY; ;
		} else if (strcmp(c, "0009") == 0) {
			*model = PI_MODEL_B ;
			*rev = PI_VERSION_2;
			*mem = 0;
			*maker = PI_MAKER_UNKNOWN;
		} else if (strcmp(c, "000d") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_2;
			*mem = 1;
			*maker = PI_MAKER_EGOMAN ;
		} else if (strcmp(c, "000e") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_2;
			*mem = 1;
			*maker = PI_MAKER_SONY;
		} else if (strcmp(c, "000f") == 0) {
			*model = PI_MODEL_B;
			*rev = PI_VERSION_2;
			*mem = 1;
			*maker = PI_MAKER_EGOMAN ;
		} else if (strcmp(c, "0010") == 0) {
			*model = PI_MODEL_BP;
			*rev = PI_VERSION_1_2;
			*mem = 1;
			*maker = PI_MAKER_SONY;
		} else if (strcmp(c, "0011") == 0) {
			*model = PI_MODEL_CM;
			*rev = PI_VERSION_1_2;
			*mem = 1;
			*maker = PI_MAKER_SONY;
		} else if (strcmp(c, "0012") == 0) {
			*model = PI_MODEL_AP;
			*rev = PI_VERSION_1_2;
			*mem = 0;
			*maker = PI_MAKER_SONY;
		} else if (strcmp(c, "0013") == 0) {
			*model = PI_MODEL_BP;
			*rev = PI_VERSION_1_2;
			*mem = 1;
			*maker = PI_MAKER_EGOMAN;
		} else if (strcmp(c, "0014") == 0) {
			*model = PI_MODEL_CM;
			*rev = PI_VERSION_1_2;
			*mem = 1;
			*maker = PI_MAKER_SONY;
		} else if (strcmp(c, "0015") == 0) {
			*model = PI_MODEL_AP;
			*rev = PI_VERSION_1_1;
			*mem = 0;
			*maker = PI_MAKER_SONY;
		} else {
			*model = 0;
			*rev = 0;
			*mem = 0;
			*maker = 0;
		}
	}
}
