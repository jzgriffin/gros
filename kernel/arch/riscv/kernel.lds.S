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

// Offset range from the global pointer that is accessible through gp-relative
// addressing.  This range is +/-2KiB, or a 12-bit signed offset, because
// immediate instruction arguments are 12 bits on RISC-V.
#define GLOBAL_POINTER_OFFSET (1 << (12 - 1))

OUTPUT(riscv)
ENTRY(_entry)

PHDRS
{
    text PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    // Text.
    . = KERNEL_BASE + KERNEL_LOAD_OFFSET;

    .text : AT(DRAM_BASE + KERNEL_LOAD_OFFSET)
    {
        __text_start = .;
        *(.text.entry);
        *(.text.unlikely .text.unlikely.*);
        *(.text.startup .text.startup.*);
        *(.text .text.*);
        __text_end = .;
    } :text

    // Read-only data.
    . = ALIGN(PAGE_SIZE);

    .rodata :
    {
        __rodata_start = .;
        *(.rodata .rodata.*);
    } :rodata

    .device.initializer :
    {
        __device_initializer_start = .;
        *(.device.initializer);
        __device_initializer_end = .;
    }
    .device.finalizer :
    {
        __device_finalizer_start = .;
        *(.device.finalizer);
        __device_finalizer_end = .;
    }

    __rodata_end = .;

    // Writable data.
    . = ALIGN(PAGE_SIZE);

    .data :
    {
        __data_start = .;
        *(.data .data.*);

    } :data
    // Small data sections must be adjacent to maximize the amount reachable
    // through gp-relative addressing.
    .sdata :
    {
        __sdata_start = .;
        *(.srodata .srodata.*);
        *(.sdata .sdata.*);
    }
    __data_end = .;

    . = .;  // Emit additional sections here.

    // The start and end of the BSS sections must be aligned at XLEN bits to
    // be efficiently cleared with XLEN-bit stores.
    . = ALIGN(__riscv_xlen / 8);
    __bss_start = .;
    .sbss :
    {
        *(.sbss .sbss.*);
    }
    .bss :
    {
        *(.bss .bss.*);
    }
    . = ALIGN(__riscv_xlen / 8);
    __bss_end = .;

    // Place the global pointer such that most of the small data is reachable.
    __global_pointer$ = __sdata_start + GLOBAL_POINTER_OFFSET;

    // End of the kernel.
    __end = .;

    /DISCARD/ :
    {
        *(.comment);
        *(.eh_frame .eh_frame.*);
    }
}
