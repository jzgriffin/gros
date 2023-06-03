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

#ifndef KERNEL_ASSEMBLER_H
#define KERNEL_ASSEMBLER_H

#ifdef __ASSEMBLER__
    #define LOCAL_FUNCTION(x) \
        .type x, @function; \
        x:; \
        .cfi_startproc
    #define END_LOCAL_FUNCTION(x) \
        .cfi_endproc; \
        .size x, . - x

    #define FUNCTION(x) \
        .global x; \
        LOCAL_FUNCTION(x)
    #define END_FUNCTION(x) END_LOCAL_FUNCTION(x)

    #define LOCAL_OBJECT(x) \
        .type x, @object; \
        x:
    #define END_LOCAL_OBJECT(x) \
        .size x, . - x

    #define OBJECT(x) \
        .global x; \
        LOCAL_OBJECT(x)
    #define END_OBJECT(x) END_LOCAL_OBJECT(x)
#endif

#endif  // KERNEL_ASSEMBLER_H
