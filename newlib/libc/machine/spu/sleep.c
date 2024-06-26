/* Copyright (c) 2008 Patrick Mansfield <patmans@us.ibm.com> */
/* Copied from libc/posix/sleep.c, removed the check for HAVE_NANOSLEEP */

/* Written 2000 by Werner Almesberger */

#include <picolibc.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>

unsigned sleep(unsigned seconds)
{
    struct timespec ts;

    ts.tv_sec = seconds;
    ts.tv_nsec = 0;
    if (!nanosleep(&ts,&ts)) return 0;
    if (errno == EINTR) return ts.tv_sec;
    return -1;
}
