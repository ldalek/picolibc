/* Copyright (C) 2005, 2007 Shaun Jackman
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

/*
FUNCTION
<<diprintf>>, <<vdiprintf>>---print to a file descriptor (integer only)

INDEX
	diprintf
INDEX
	_diprintf_r
INDEX
	vdiprintf
INDEX
	_vdiprintf_r

SYNOPSIS
	#include <stdio.h>
	#include <stdarg.h>
	int diprintf(int <[fd]>, const char *<[format]>, ...);
	int vdiprintf(int <[fd]>, const char *<[format]>, va_list <[ap]>);
	int diprintf( int <[fd]>,
			const char *<[format]>, ...);
	int vdiprintf( int <[fd]>,
			const char *<[format]>, va_list <[ap]>);

DESCRIPTION
<<diprintf>> and <<vdiprintf>> are similar to <<dprintf>> and <<vdprintf>>,
except that only integer format specifiers are processed.

The functions <<_diprintf_r>> and <<_vdiprintf_r>> are simply
reentrant versions of the functions above.

RETURNS
Similar to <<dprintf>> and <<vdprintf>>.

PORTABILITY
This set of functions is an integer-only extension, and is not portable.

Supporting OS subroutines required: <<sbrk>>, <<write>>.
*/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

int
diprintf (int fd,
       const char *format, ...)
{
  va_list ap;
  int n;

  va_start (ap, format);
  n = vdiprintf ( fd, format, ap);
  va_end (ap);
  return n;
}
