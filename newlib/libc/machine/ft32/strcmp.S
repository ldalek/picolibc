/* A strcmp.c for FT32
   Copyright (C) 2014 FTDI (support@ftdichip.com)

   The authors hereby grant permission to use, copy, modify, distribute,
   and license this software and its documentation for any purpose, provided
   that existing copyright notices are retained in all copies and that this
   notice is included verbatim in any distributions. No written agreement,
   license, or royalty fee is required for any of the authorized uses.
   Modifications to this software may be copyrighted by their authors
   and need not follow the licensing terms described here, provided that
   the new terms are clearly indicated on the first page of each file where
   they apply.  */

#include <picolibc.h>

        .text
        .global        strcmp
        .type        strcmp,@function
strcmp:
        strcmp.b $r0,$r0,$r1
        return
.Lend2:
        .size        strcmp,.Lend2-strcmp

