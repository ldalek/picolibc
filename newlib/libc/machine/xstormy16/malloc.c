/* stdlib/malloc.c defines all these symbols in this file. */
#include <sys/features.h>

#define DEFINE_FREE
#define DEFINE_MALLOC
#include "tiny-malloc.c"
