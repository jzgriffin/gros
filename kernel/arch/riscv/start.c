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

#include <kernel/arch/halt.h>
#include <kernel/main.h>

#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <string.h>

extern uint8_t* __bss_start;
extern uint8_t* __bss_end;

static void _initialize_bss(void)
{
    memset(__bss_start, 0, __bss_end - __bss_start);
}

noreturn void _start(size_t hart_id, void* device_tree)
{
    (void)hart_id;
    (void)device_tree;

    _initialize_bss();

    const int exit_code = main();
    (void)exit_code;

    halt();
}
