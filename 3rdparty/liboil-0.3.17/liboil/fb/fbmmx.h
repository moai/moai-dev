/*
 * Copyright © 2004 Red Hat, Inc.
 * Copyright © 2005 Trolltech AS
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Red Hat not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Red Hat makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * Author:  Søren Sandmann (sandmann@redhat.com)
 *          Lars Knoll (lars@trolltech.com)
 * 
 * Based on work by Owen Taylor
 */


#define MmxNegate(x) _mm_xor_si64((x), mmx_4x00ff)
#define MmxAlpha(x) _mm_shuffle_pi16 ((x), _MM_SHUFFLE(3, 3, 3, 3));
#define MmxTo(x) _mm_unpacklo_pi8 (_mm_cvtsi32_si64((x)), mmx_0)
#define MmxFrom(x) (uint32_t)_mm_cvtsi64_si32(_mm_packs_pu16((x), mmx_0))
#define MmxAdd(x, y) _mm_adds_pu16 ((x), (y))

#define MmxMulAdd(x, a, y) do {                     \
        x = _mm_mullo_pi16 (x, a);                  \
        x = _mm_adds_pu16 (x, _mm_srli_pi16 (x, 8)); \
        x = _mm_adds_pu16 (x, mmx_4x0080);           \
        x = _mm_srli_pi16 (x, 8);                   \
        x = _mm_adds_pu16(x, y);                    \
    } while (0)

#define MmxMul(x, a) do {                           \
        x = _mm_mullo_pi16 (x, a);                  \
        x = _mm_adds_pu16 (x, _mm_srli_pi16 (x, 8)); \
        x = _mm_adds_pu16 (x, mmx_4x0080);           \
        x = _mm_srli_pi16 (x, 8);                   \
    } while (0)

#define MmxAddMul(x, a, y, b) do {                  \
        x = _mm_mullo_pi16 (x, a);                  \
        y = _mm_mullo_pi16 (y, b);                  \
        x = _mm_srli_pi16(x, 1);                    \
        y = _mm_srli_pi16(y, 1);                    \
        x = _mm_adds_pu16 (x, y);                    \
        x = _mm_adds_pu16 (x, _mm_srli_pi16 (x, 8)); \
        x = _mm_adds_pu16 (x, mmx_4x0080);           \
        x = _mm_srli_pi16 (x, 7);                   \
    } while (0)

