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

#ifndef KERNEL_ARCH_MEMORY_H
#define KERNEL_ARCH_MEMORY_H

#include <kernel/arch/types.h>
#include <kernel/config.h>

#if RISCV_MMU_BARE
    #define KERNEL_BASE LITERAL_UX(DRAM_BASE)
#else
    #if RISCV_MMU_SV32
        #error MMU Sv32 is not implemented
    #elif RISCV_MMU_SV39  // Two 38-bit halves.
        #define USER_SPACE_BASE   LITERAL_UX(0x0000000000000000)
        #define USER_SPACE_SIZE   LITERAL_UX(0x0000004000000000)
        #define KERNEL_SPACE_BASE LITERAL_UX(0xFFFFFFC000000000)
        #define KERNEL_SPACE_SIZE LITERAL_UX(0x0000004000000000)
    #elif RISCV_MMU_SV48
        #error MMU Sv48 is not implemented
    #else
        #error Unknown MMU
    #endif
    #define KERNEL_BASE LITERAL_UX(KERNEL_SPACE_BASE) + LITERAL_UX(DRAM_BASE)
#endif

#define PAGE_BITS LITERAL_UX(12)
#define PAGE_SIZE BIT_UX(PAGE_BITS)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define ROUND_PAGE_DOWN(x) (((x) >> PAGE_BITS) << PAGE_BITS)
#define ROUND_PAGE_UP(x) (ROUND_PAGE_DOWN((x) + (PAGE_SIZE - 1)))

#define STACK_SIZE PAGE_SIZE

#ifdef __C__
    typedef uint_xlen_t PhysicalAddress;
#endif

#endif  // KERNEL_ARCH_MEMORY_H
