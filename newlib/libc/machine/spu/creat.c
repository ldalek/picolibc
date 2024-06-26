/* Copyright (c) 2007 Patrick Mansfield <patmans@us.ibm.com> */
/* creat() "system call" */

/* copied from libc/posix/creat.c */

/* This is needed by f2c and therefore the SPEC benchmarks.  */

#include <picolibc.h>

#include <fcntl.h>

int
creat (const char *path,
       mode_t mode)
{
  return open (path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
