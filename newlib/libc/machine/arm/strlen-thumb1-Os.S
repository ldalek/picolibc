/* Copyright (c) 2015 ARM Ltd.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
	 notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
	 notice, this list of conditions and the following disclaimer in the
	 documentation and/or other materials provided with the distribution.
       * Neither the name of the Linaro nor the
	 names of its contributors may be used to endorse or promote products
	 derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */

#include <picolibc.h>

	.macro def_fn f p2align=0
	.text
	.p2align \p2align
	.global \f
	.type \f, %function
\f:
	.endm

	.arch	armv4t
	.eabi_attribute Tag_also_compatible_with, "\006\013" /* ARMv6-M.  */
	.eabi_attribute Tag_ARM_ISA_use, 0
	.thumb
	.syntax unified

def_fn	strlen p2align=1
	movs	r3, #0
1:
	ldrb	r2, [r0, r3]
	adds	r3, r3, #1
	cmp	r2, #0
	bne	1b
	subs	r0, r3, #1
	bx	lr
	.size	strlen, . - strlen


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
