/*
 * ====================================================
 * Copyright (C) 1998, 2002, 2008 by Red Hat Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <picolibc.h>

	#include "i386mach.h"

	.global SYM (memset)
       SOTYPE_FUNCTION(memset)

SYM (memset):

#ifdef __iamcu__
	pushl edi
	movl eax,edi
	movzbl dl,eax
	mov edi,edx
	rep stosb
	mov edx,eax
	popl edi
#else
	pushl ebp
	movl esp,ebp
	pushl edi
	movl 8(ebp),edi
	movzbl 12(ebp),eax
	movl 16(ebp),ecx
	cld

#ifndef __OPTIMIZE_SIZE__
/* Less than 16 bytes won't benefit from the 'rep stosl' loop.  */
	cmpl $16,ecx
	jbe .L19
	testl $7,edi
	je .L10

/* It turns out that 8-byte aligned 'rep stosl' outperforms
   4-byte aligned on some x86 platforms.  */
	movb al,(edi)
	incl edi
	decl ecx
	testl $7,edi
	je .L10

	movb al,(edi)
	incl edi
	decl ecx
	testl $7,edi
	je .L10

	movb al,(edi)
	incl edi
	decl ecx
	testl $7,edi
	je .L10

	movb al,(edi)
	incl edi
	decl ecx
	testl $7,edi
	je .L10

	movb al,(edi)
	incl edi
	decl ecx
	testl $7,edi
	je .L10

	movb al,(edi)
	incl edi
	decl ecx
	testl $7,edi
	je .L10

	movb al,(edi)
	incl edi
	decl ecx

/* At this point, ecx>8 and edi%8==0.  */
.L10:
	movb al,ah
	movl eax,edx
	sall $16,edx
	orl edx,eax

	movl ecx,edx
	shrl $2,ecx
	andl $3,edx
	rep
	stosl
	movl edx,ecx
#endif /* not __OPTIMIZE_SIZE__ */

.L19:
	rep
	stosb

	movl 8(ebp),eax

	leal -4(ebp),esp
	popl edi
	leave
#endif
	ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
