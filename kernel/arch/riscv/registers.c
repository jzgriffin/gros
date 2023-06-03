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

#include <kernel/arch/registers.h>

Registers copy_registers(void)
{
    Registers registers;
#if RISCV_EXT_I
    __asm__(ASM_SX " x0, %0" : "=o" (registers.x[0]) : :);
    __asm__(ASM_SX " x1, %0" : "=o" (registers.x[1]) : :);
    __asm__(ASM_SX " x2, %0" : "=o" (registers.x[2]) : :);
    __asm__(ASM_SX " x3, %0" : "=o" (registers.x[3]) : :);
    __asm__(ASM_SX " x4, %0" : "=o" (registers.x[4]) : :);
    __asm__(ASM_SX " x5, %0" : "=o" (registers.x[5]) : :);
    __asm__(ASM_SX " x6, %0" : "=o" (registers.x[6]) : :);
    __asm__(ASM_SX " x7, %0" : "=o" (registers.x[7]) : :);
    __asm__(ASM_SX " x8, %0" : "=o" (registers.x[8]) : :);
    __asm__(ASM_SX " x9, %0" : "=o" (registers.x[9]) : :);
    __asm__(ASM_SX " x10, %0" : "=o" (registers.x[10]) : :);
    __asm__(ASM_SX " x11, %0" : "=o" (registers.x[11]) : :);
    __asm__(ASM_SX " x12, %0" : "=o" (registers.x[12]) : :);
    __asm__(ASM_SX " x13, %0" : "=o" (registers.x[13]) : :);
    __asm__(ASM_SX " x14, %0" : "=o" (registers.x[14]) : :);
    __asm__(ASM_SX " x15, %0" : "=o" (registers.x[15]) : :);
    __asm__(ASM_SX " x16, %0" : "=o" (registers.x[16]) : :);
    __asm__(ASM_SX " x17, %0" : "=o" (registers.x[17]) : :);
    __asm__(ASM_SX " x18, %0" : "=o" (registers.x[18]) : :);
    __asm__(ASM_SX " x19, %0" : "=o" (registers.x[19]) : :);
    __asm__(ASM_SX " x20, %0" : "=o" (registers.x[20]) : :);
    __asm__(ASM_SX " x21, %0" : "=o" (registers.x[21]) : :);
    __asm__(ASM_SX " x22, %0" : "=o" (registers.x[22]) : :);
    __asm__(ASM_SX " x23, %0" : "=o" (registers.x[23]) : :);
    __asm__(ASM_SX " x24, %0" : "=o" (registers.x[24]) : :);
    __asm__(ASM_SX " x25, %0" : "=o" (registers.x[25]) : :);
    __asm__(ASM_SX " x26, %0" : "=o" (registers.x[26]) : :);
    __asm__(ASM_SX " x27, %0" : "=o" (registers.x[27]) : :);
    __asm__(ASM_SX " x28, %0" : "=o" (registers.x[28]) : :);
    __asm__(ASM_SX " x29, %0" : "=o" (registers.x[29]) : :);
    __asm__(ASM_SX " x30, %0" : "=o" (registers.x[30]) : :);
    __asm__(ASM_SX " x31, %0" : "=o" (registers.x[31]) : :);
    __asm__("auipc t0, 0\n"
            ASM_SX " t0, %0" : "=o" (registers.pc) : :);
#endif

#if RISCV_EXT_F
    __asm__(ASM_FSX " f0, %0" : "=o" (registers.f[0]) : :);
    __asm__(ASM_FSX " f1, %0" : "=o" (registers.f[1]) : :);
    __asm__(ASM_FSX " f2, %0" : "=o" (registers.f[2]) : :);
    __asm__(ASM_FSX " f3, %0" : "=o" (registers.f[3]) : :);
    __asm__(ASM_FSX " f4, %0" : "=o" (registers.f[4]) : :);
    __asm__(ASM_FSX " f5, %0" : "=o" (registers.f[5]) : :);
    __asm__(ASM_FSX " f6, %0" : "=o" (registers.f[6]) : :);
    __asm__(ASM_FSX " f7, %0" : "=o" (registers.f[7]) : :);
    __asm__(ASM_FSX " f8, %0" : "=o" (registers.f[8]) : :);
    __asm__(ASM_FSX " f9, %0" : "=o" (registers.f[9]) : :);
    __asm__(ASM_FSX " f10, %0" : "=o" (registers.f[10]) : :);
    __asm__(ASM_FSX " f11, %0" : "=o" (registers.f[11]) : :);
    __asm__(ASM_FSX " f12, %0" : "=o" (registers.f[12]) : :);
    __asm__(ASM_FSX " f13, %0" : "=o" (registers.f[13]) : :);
    __asm__(ASM_FSX " f14, %0" : "=o" (registers.f[14]) : :);
    __asm__(ASM_FSX " f15, %0" : "=o" (registers.f[15]) : :);
    __asm__(ASM_FSX " f16, %0" : "=o" (registers.f[16]) : :);
    __asm__(ASM_FSX " f17, %0" : "=o" (registers.f[17]) : :);
    __asm__(ASM_FSX " f18, %0" : "=o" (registers.f[18]) : :);
    __asm__(ASM_FSX " f19, %0" : "=o" (registers.f[19]) : :);
    __asm__(ASM_FSX " f20, %0" : "=o" (registers.f[20]) : :);
    __asm__(ASM_FSX " f21, %0" : "=o" (registers.f[21]) : :);
    __asm__(ASM_FSX " f22, %0" : "=o" (registers.f[22]) : :);
    __asm__(ASM_FSX " f23, %0" : "=o" (registers.f[23]) : :);
    __asm__(ASM_FSX " f24, %0" : "=o" (registers.f[24]) : :);
    __asm__(ASM_FSX " f25, %0" : "=o" (registers.f[25]) : :);
    __asm__(ASM_FSX " f26, %0" : "=o" (registers.f[26]) : :);
    __asm__(ASM_FSX " f27, %0" : "=o" (registers.f[27]) : :);
    __asm__(ASM_FSX " f28, %0" : "=o" (registers.f[28]) : :);
    __asm__(ASM_FSX " f29, %0" : "=o" (registers.f[29]) : :);
    __asm__(ASM_FSX " f30, %0" : "=o" (registers.f[30]) : :);
    __asm__(ASM_FSX " f31, %0" : "=o" (registers.f[31]) : :);
    __asm__("frcsr %0" : "=r" (registers.fcsr) ::);
#endif

    __asm__("csrr %0, sstatus" : "=r" (registers.sstatus) ::);
    __asm__("csrr %0, stvec" : "=r" (registers.stvec) ::);
    __asm__("csrr %0, sip" : "=r" (registers.sip) ::);
    __asm__("csrr %0, sie" : "=r" (registers.sie) ::);
    __asm__("csrr %0, sscratch" : "=r" (registers.sscratch) ::);
    __asm__("csrr %0, sepc" : "=r" (registers.sepc) ::);
    __asm__("csrr %0, scause" : "=r" (registers.scause) ::);
    __asm__("csrr %0, stval" : "=r" (registers.stval) ::);
    __asm__("csrr %0, senvcfg" : "=r" (registers.senvcfg) ::);
    __asm__("csrr %0, satp" : "=r" (registers.satp) ::);
    return registers;
}
