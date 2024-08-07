/*
FUNCTION
	<<wcslcpy>>---copy a wide-character string to specified length

SYNOPSIS
	#include <wchar.h>
	size_t wcslcpy(wchar_t *<[dst]>, const wchar_t *<[src]>, size_t <[siz]>);

DESCRIPTION
	<<wcslcpy>> copies wide characters from <[src]> to <[dst]>
	such that up to <[siz]> - 1 characters are copied.  A
	terminating null is appended to the result, unless <[siz]>
	is zero.

RETURNS
	<<wcslcpy>> returns the number of wide characters in <[src]>,
	not including the terminating null wide character.  If the
	return value is greater than or equal to <[siz]>, then
	not all wide characters were copied from <[src]> and truncation
	occurred.

PORTABILITY
No supporting OS subroutines are required.
*/

/*      $OpenBSD: wcslcpy.c,v 1.8 2019/01/25 00:19:25 millert Exp $     */

/*
 * Copyright (c) 1998, 2015 Todd C. Miller <millert@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _GNU_SOURCE
#include <wchar.h>

/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns wcslen(src); if retval >= dsize, truncation occurred.
 */
size_t
wcslcpy (wchar_t *dst,
        const wchar_t *src,
        size_t dsize)
{
        const wchar_t *osrc = src;
        size_t nleft = dsize;

        /* Copy as many bytes as will fit. */
        if (nleft != 0) {
                while (--nleft != 0) {
                        if ((*dst++ = *src++) == L'\0')
                                break;
                }
        }

        /* Not enough room in dst, add NUL and traverse rest of src. */
        if (nleft == 0) {
                if (dsize != 0)
                        *dst = L'\0';           /* NUL-terminate dst */
                while (*src++)
                        ;
        }

        return(src - osrc - 1); /* count does not include NUL */
}
