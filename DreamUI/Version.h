#include "stdafx.h"
#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

/*
static int DateVerCalc () {
	unsigned char month, day, year;
	const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
							"Sep", "Oct", "Nov", "Dec"};

  char temp [] = __DATE__;
  unsigned char i;

  year = atoi(temp + 9);
  *(temp + 6) = 0;
  day = atoi(temp + 4);
  *(temp + 3) = 0;
  for (i = 0; i < 12; i++)
  {
	if (!strcmp(temp, months[i]))
	{
	  month = i + 1;
	  return day + (month * 100) ;//+ (year * 10000);
	}
  }
  return 0;
}
*/


const struct DreamWarcraftVersionInfo {
	unsigned int	main;
	unsigned int	release;
	unsigned int	build;
	unsigned int	revision;
} VERSION =


{//µ±Ç°°æ±¾
	3,
	18,
	2,
	0
};

#endif