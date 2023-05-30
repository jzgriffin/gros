/* Copyright (c) 2023 Jeremiah Z. Griffin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef KERNEL_ARCH_TYPES_H
#define KERNEL_ARCH_TYPES_H

#include <kernel/config.h>

#ifdef __C__
#include <stdint.h>
#endif

#if RISCV_XLEN == 32
#   if defined(__C__)
typedef uint32_t uint_xlen_t;
#       define ASM_LX "lw"
#       define ASM_SX "sw"
#   elif defined(__ASSEMBLER__)
#       define LX lw
#       define SX sw
#   endif
#elif RISCV_XLEN == 64
#   if defined(__C__)
typedef uint64_t uint_xlen_t;
#       define ASM_LX "ld"
#       define ASM_SX "sd"
#   elif defined(__ASSEMBLER__)
#       define LX ld
#       define SX sd
#   endif
#else
#   error Unknown XLEN
#endif

#if RISCV_FLEN == 32
#   if defined(__C__)
typedef uint32_t uint_flen_t;
typedef float float_flen_t;
#       define ASM_FLX "fld"
#       define ASM_FSX "fsd"
#   elif defined(__ASSEMBLER__)
#       define FLX fld
#       define FSX fsd
#   endif
#elif RISCV_FLEN == 64
#   if defined(__C__)
typedef uint64_t uint_flen_t;
typedef double float_flen_t;
#       define ASM_FLX "fld"
#       define ASM_FSX "fsd"
#   elif defined(__ASSEMBLER__)
#       define FLX fld
#       define FSX fsd
#   endif
#else
#   error Unknown FLEN
#endif

#endif  /* KERNEL_ARCH_TYPES_H */
