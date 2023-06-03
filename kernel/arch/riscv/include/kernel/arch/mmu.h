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

#ifndef KERNEL_ARCH_MMU_H
#define KERNEL_ARCH_MMU_H

#include <kernel/config.h>

#if RISCV_MMU_BARE
#    define KERNEL_BASE DRAM_BASE
#else
#    if RISCV_MMU_SV32
#        error MMU Sv32 is not implemented
#    elif RISCV_MMU_SV39  /* Two 38-bit halves. */
#        define USER_SPACE_BASE   0x0000000000000000
#        define USER_SPACE_SIZE   0x0000004000000000
#        define KERNEL_SPACE_BASE 0xFFFFFFC000000000
#        define KERNEL_SPACE_SIZE 0x0000004000000000
#    elif RISCV_MMU_SV48
#        error MMU Sv48 is not implemented
#    else
#        error Unknown MMU
#    endif
#    define KERNEL_BASE KERNEL_SPACE_BASE + DRAM_BASE
#endif

#define PAGE_BITS 12
#define PAGE_SIZE (1 << PAGE_BITS)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#endif  /* KERNEL_ARCH_MMU_H */
