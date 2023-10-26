/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2023 Keith Packard
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Convert a 32-bit or 64-bit float to a string in hex ('a') format
 *
 * This chunk of code gets inserted into the vfprintf function in the
 * double/float handling code as well as the long double case when
 * long double is 32- or 64- bits.
 *
 * This code assumes that there is an integer type suitable for holding
 * the entire floating point value.
 *
 * Define DTOX_UINT, DTOX_INT, DTOX_FLOAT and DTOX_SIZE before including
 * this file.
 */

{
    DTOX_INT   fi, s;

    c = 'p';
    flags |= FL_FLTEXP | FL_FLTHEX;

#if DTOX_SIZE == 4
#define EXP_SHIFT       23
#define EXP_MASK        0xff
#define SIG_SHIFT       1
#define SIG_MASK        0x7fffff
#define EXP_BIAS        127
#define ASUINT(x)       asuint(x)
    ndigs = 7;
#else
#define EXP_SHIFT       52
#define EXP_MASK        0x7ff
#define SIG_SHIFT       0
#define SIG_MASK        0xfffffffffffffLL
#define EXP_BIAS        1023
#define ASUINT(x)       asuint64(x)
    ndigs = 14;
#endif

    fi = ASUINT(fval);

    _dtoa.digits[0] = '0';

    exp = ((fi >> EXP_SHIFT) & EXP_MASK);
    s = (fi & SIG_MASK) << SIG_SHIFT;
    if (s | exp) {
        if (!exp)
            exp = 1;
        else
            _dtoa.digits[0] = '1';
        exp -= EXP_BIAS;
    }
    _dtoa.flags = 0;
    if (fi < 0)
        _dtoa.flags = DTOA_MINUS;

    if (!(flags & FL_PREC))
        prec = 0;
    else if (prec >= (ndigs - 1))
        prec = ndigs - 1;
    else {
        int                 bits = ((ndigs - 1) - prec) << 2;
        DTOX_INT            half = ((DTOX_INT) 1) << (bits - 1);
        DTOX_INT            mask = ~((half << 1) - 1);

        /* round even */
        if ((s & ~mask) > half || ((s >> bits) & 1) != 0)
            s += half;
        /* special case rounding first digit */
        if (s > (SIG_MASK << SIG_SHIFT))
            _dtoa.digits[0]++;
        s &= mask;
    }

    if (exp == EXP_BIAS + 1) {
        if (s)
            _dtoa.flags |= DTOA_NAN;
        else
            _dtoa.flags |= DTOA_INF;
    } else {
        int d;
        for (d = ndigs - 1; d; d--) {
            int dig = s & 0xf;
            s >>= 4;
            if (dig == 0 && d > prec)
                continue;
            if (dig <= 9)
                dig += '0';
            else
                dig += TOCASE('a' - 10);
            _dtoa.digits[d] = dig;
            if (prec < d)
                prec = d;
        }
    }
    ndigs_exp = 1;
}

#undef EXP_INF
#undef SIG_BITS
#undef EXP_SHIFT
#undef EXP_MASK
#undef SIG_SHIFT
#undef SIG_MASK
#undef EXP_BIAS
#undef ASUINT

#undef DTOX_SIZE
#undef DTOX_UINT
#undef DTOX_INT
#undef DTOX_FLOAT
