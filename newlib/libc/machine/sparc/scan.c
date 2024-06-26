/*
Copyright (c) 1990 The Regents of the University of California.
All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
and/or other materials related to such
distribution and use acknowledge that the software was developed
by the University of California, Berkeley.  The name of the
University may not be used to endorse or promote products derived
from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* Cover function to sparclet `scan' instruction.

   This file is in the public domain.  */

#include <picolibc.h>

#ifdef __sparclet__

int
scan (int a, int b)
{
  int res;
  __asm__ ("scan %1,%2,%0" : "=r" (res) : "r" (a), "r" (b));
  return res;
}

#endif
