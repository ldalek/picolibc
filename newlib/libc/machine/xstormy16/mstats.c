/* stdlib/mstats.c defines all these symbols in this file.
   TODO: Missing mstats function.  */
#include <picolibc.h>

#define DEFINE_MALLINFO
#define DEFINE_MALLOC_STATS
#define DEFINE_MALLOPT
#include "tiny-malloc.c"
