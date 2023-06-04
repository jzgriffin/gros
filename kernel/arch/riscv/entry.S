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

#include <kernel/arch/memory.h>
#include <kernel/arch/types.h>
#include <kernel/assembler.h>

.section .text.entry
FUNCTION(_entry)
    // This function receives the following parameters in supervisor mode:
    //   a0 - Hart ID
    //   a1 - Address of device tree structure
    // These must be forwarded to _start.  They are already in the correct
    // registers for every RISC-V ABI variant.  Any changes to these registers
    // must be undone before calling _start.

    // Set the global pointer with relax turned off.  This prevents the load
    // of __global_pointer$ from being relaxed -- i.e., loaded relative to the
    // current global pointer.
.option push
.option norelax
    lla     gp, __global_pointer$
.option pop

    // Set the stack pointer.
    lla     sp, _stack_top

    // Clear the BSS section.  NOTE: __bss_start/_bss_end must be aligned at
    // XLEN bits for SX to be efficient across implementations.
    lla     t0, __bss_start
    lla     t1, __bss_end
    bgeu    t0, t1, 2f
1:  SX      zero, (t0)
    addi    t0, t0, __riscv_xlen / 8
    bltu    t0, t1, 1b
2:

    // Tail call into the C start function which does architecture-specific
    // initialization, calls main, and enters a halting loop if main ever
    // returns.
    tail _start
END_FUNCTION(_entry)

// Reserve the stack.
.section .bss
.align __riscv_xlen_bytes
OBJECT(_stack)
    .skip   STACK_SIZE
END_OBJECT(_stack)
OBJECT(_stack_top)
END_OBJECT(_stack_top)
