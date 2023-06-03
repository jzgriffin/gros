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

#include <kernel/arch/mmu.h>

OUTPUT(riscv)
ENTRY(_entry)

SECTIONS
{
    . = KERNEL_BASE + KERNEL_LOAD_OFFSET;

    .text : AT(DRAM_BASE + KERNEL_LOAD_OFFSET)
    {
        *(.entry.text);
        *(.text.unlikely .text.unlikely.*);
        *(.text.startup .text.startup.*);
        *(.text .text.*);
    }

    . = ALIGN(PAGE_SIZE);

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

    .rodata :
    {
        *(.rodata .rodata.*);
    }

    .data :
    {
        *(.data .data.*);
    }

    // Position the global pointer one half-page after the start of sdata,
    // which is followed by sbss, to maximize the amount of both that can be
    // reached through short addressing.
    .sdata :
    {
        __global_pointer$ = . + (PAGE_SIZE / 2);
        *(.sdata .sdata.*);
    }

    // Align sbss at XLEN bits instead of a page boundary to maximize the
    // amount reachable through short addressing.
    . = ALIGN(RISCV_XLEN / 8);

    __bss_start = .;

    .sbss :
    {
        *(.sbss .sbss.*);
        . += (PAGE_SIZE / 2);  // Reserve one half-page of stack.
        __stack_pointer = .;
    }

    .bss :
    {
        *(.bss .bss.*);
    }

    __bss_end = .;

    /DISCARD/ :
    {
        *(.comment);
        *(.eh_frame .eh_frame.*);
    }
}
