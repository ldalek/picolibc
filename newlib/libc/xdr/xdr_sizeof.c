/*
 * Copyright (c) 2009, Sun Microsystems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Sun Microsystems, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * xdr_sizeof.c
 *
 * Copyright 1990 Sun Microsystems, Inc.
 *
 * General purpose routine to see how much space something will use
 * when serialized using XDR.
 */

#define _DEFAULT_SOURCE
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <sys/types.h>
#include <stdlib.h>

#include "xdr_private.h"

/* ARGSUSED */
static bool_t
x_putlong (XDR * xdrs,
	const long *longp)
{
  (void) longp;
  xdrs->x_handy += BYTES_PER_XDR_UNIT;
  return TRUE;
}

/* ARGSUSED */
static bool_t
x_putbytes (XDR * xdrs,
	const char *bp,
	u_int len)
{
  (void) bp;
  xdrs->x_handy += len;
  return TRUE;
}

static u_int
x_getpostn (XDR * xdrs)
{
  return xdrs->x_handy;
}

/* ARGSUSED */
static bool_t
x_setpostn (XDR * xdrs,
	u_int pos)
{
  (void) xdrs;
  (void) pos;
  /* This is not allowed */
  return FALSE;
}

#ifdef __GNUCLIKE_PRAGMA_DIAGNOSTIC
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
/* 'len' is used directly with calloc which confuses -fanalyzer */
#pragma GCC diagnostic ignored "-Wanalyzer-allocation-size"
#endif

static int32_t *
x_inline (XDR * xdrs,
	u_int len)
{
  if (len == 0)
    return NULL;
  if (xdrs->x_op != XDR_ENCODE)
    return NULL;
  if (len < (u_int) (uintptr_t) xdrs->x_base)
    {
      /* x_private was already allocated */
      xdrs->x_handy += len;
      return (int32_t *) xdrs->x_private;
    }
  else
    {
      /* Free the earlier space and allocate new area */
      if (xdrs->x_private)
        mem_free (xdrs->x_private, sizeof (xdrs->x_private));
      if ((xdrs->x_private = (caddr_t) mem_alloc (len)) == NULL)
        {
          xdrs->x_base = 0;
          return NULL;
        }
      xdrs->x_base = (caddr_t) (intptr_t) len;
      xdrs->x_handy += len;
      return (int32_t *) xdrs->x_private;
    }
}

static int
harmless (void)
{
  /* Always return FALSE/NULL, as the case may be */
  return 0;
}

static void
x_destroy (XDR * xdrs)
{
  xdrs->x_handy = 0;
  xdrs->x_base = 0;
  if (xdrs->x_private)
    {
      mem_free (xdrs->x_private, sizeof (xdrs->x_private));
      xdrs->x_private = NULL;
    }
  return;
}

static bool_t
x_putint32 (XDR *xdrs,
	const int32_t *int32p)
{
  (void) int32p;
  xdrs->x_handy += BYTES_PER_XDR_UNIT;
  return TRUE;
}


unsigned long
xdr_sizeof (xdrproc_t func,
	void *data)
{
  XDR x;
  struct xdr_ops ops;
  bool_t stat;
  /* to stop ANSI-C compiler from complaining */
  typedef bool_t (*dummyfunc1) (XDR *, long *);
  typedef bool_t (*dummyfunc2) (XDR *, caddr_t, u_int);
  typedef bool_t (*dummyfunc3) (XDR *, int32_t *);

  ops.x_putlong = x_putlong;
  ops.x_putbytes = x_putbytes;
  ops.x_inline = x_inline;
  ops.x_getpostn = x_getpostn;
  ops.x_setpostn = x_setpostn;
  ops.x_destroy = x_destroy;
  ops.x_putint32 = x_putint32;

  /* the other harmless ones */
  ops.x_getlong = (dummyfunc1) (void *) harmless;
  ops.x_getbytes = (dummyfunc2) (void *) harmless;
  ops.x_getint32 = (dummyfunc3) (void *) harmless;

  x.x_op = XDR_ENCODE;
  x.x_ops = &ops;
  x.x_handy = 0;
  x.x_private = (caddr_t) NULL;
  x.x_base = (caddr_t) 0;

  stat = func (&x, data);
  if (x.x_private)
    mem_free (x.x_private, sizeof (x.x_private));
  return (stat == TRUE ? x.x_handy : 0);
}

