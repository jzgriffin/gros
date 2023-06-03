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

#include <kernel/arch/dump.h>

#include <kernel/console.h>

void dump_registers(const Registers* registers)
{
    // Integer registers.
#if RISCV_EXT_I
    for (size_t i = 0; i < sizeof registers->x / sizeof *registers->x; ++i) {
        if (i % 4 == 0) {
            if (i > 0) {
                cprintf("\n");
            }
        }
        else {
            cprintf("\t");
        }
        cprintf("x%u = %X", i, registers->x[i]);
    }
    cprintf("\n");
    cprintf("pc = %X\n", registers->pc);
#endif

    // Floating point registers.
#if RISCV_EXT_I
    for (size_t i = 0; i < sizeof registers->f / sizeof *registers->f; ++i) {
        if (i % 4 == 0) {
            if (i > 0) {
                cprintf("\n");
            }
        }
        else {
            cprintf("\t");
        }
        cprintf("f%u = %X", i, registers->f[i]);
    }
    cprintf("\n");
    cprintf("fcsr = %X\n", registers->fcsr);
#endif

    // Supervisor registers.
    cprintf("sstatus = %X\t", registers->sstatus);
    cprintf("stvec = %X\t", registers->stvec);
    cprintf("sip = %X\t", registers->sip);
    cprintf("sie = %X\n", registers->sie);
    cprintf("sscratch = %X\t", registers->sscratch);
    cprintf("sepc = %X\t", registers->sepc);
    cprintf("scause = %X\t", registers->scause);
    cprintf("stval = %X\n", registers->stval);
    cprintf("senvcfg = %X\t", registers->senvcfg);
    cprintf("satp = %X\n", registers->satp);
}
