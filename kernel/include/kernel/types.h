// Copyright (c) 2023 Jeremiah Z. Griffin
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#ifdef __C__
    #define _LITERAL(x, t) x ## t
    #define LITERAL(x, t) _LITERAL(x, t)
#else
    #define LITERAL(x, t) x
#endif

#define BIT(x, t) (LITERAL(1, t) << (x))

#define LITERAL_L(x)   LITERAL(x, l)
#define LITERAL_LL(x)  LITERAL(x, ll)
#define LITERAL_UL(x)  LITERAL(x, ul)
#define LITERAL_ULL(x) LITERAL(x, ull)

#define BIT_L(x)   BIT(x, l)
#define BIT_LL(x)  BIT(x, ll)
#define BIT_UL(x)  BIT(x, ul)
#define BIT_ULL(x) BIT(x, ull)

#endif  // KERNEL_TYPES_H
