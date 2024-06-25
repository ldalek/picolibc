/* stdlib/valloc.c defines all these symbols in this file. */
#include <sys/features.h>

#define DEFINE_PVALLOC
#define DEFINE_VALLOC
#include "tiny-malloc.c"
