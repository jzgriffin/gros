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

.section .entry.text
.global _entry
.type _entry, @function
_entry:
.cfi_startproc
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
    la gp, __global_pointer$
.option pop

    // Set the stack pointer.
    la sp, __stack_pointer

    // Tail call into the C start function which does architecture-specific
    // initialization, calls main, and enters a halting loop if main ever
    // returns.
    tail _start
.cfi_endproc
