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

#ifndef KERNEL_ARCH_REGISTERS_H
#define KERNEL_ARCH_REGISTERS_H

#include <kernel/arch/types.h>
#include <kernel/config.h>

typedef struct Registers
{
    // Integer registers.
#if RISCV_EXT_I
    uint_xlen_t x[32];
    uint_xlen_t pc;
#endif

    // Floating point registers.
#if RISCV_EXT_F
    uint_flen_t f[32];
    uint32_t fcsr;
#endif

    // Supervisor registers.
    uint_xlen_t sstatus;
    uint_xlen_t stvec;
    uint_xlen_t sip;
    uint_xlen_t sie;
    uint_xlen_t sscratch;
    uint_xlen_t sepc;
    uint_xlen_t scause;
    uint_xlen_t stval;
    uint_xlen_t senvcfg;
    uint_xlen_t satp;
} Registers;

Registers copy_registers(void);

#endif  // KERNEL_ARCH_REGISTERS_H
