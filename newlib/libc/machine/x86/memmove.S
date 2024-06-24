#include <sys/features.h>

#ifndef __x86_64
#include "memmove-32.S"
#endif
