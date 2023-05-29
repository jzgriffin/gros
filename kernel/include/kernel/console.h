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

#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CONSOLE_NAME_SIZE 32

typedef struct Console
{
    char name[CONSOLE_NAME_SIZE];
    const void* tag;
    void (*activate)(const struct Console*);
    void (*deactivate)(const struct Console*);
    size_t (*read)(const struct Console*, char*, size_t);
    size_t (*write)(const struct Console*, const char*);
    bool (*put)(const struct Console*, char);
} Console;

bool register_console(const Console* console);
bool deregister_console(const Console* console);
size_t get_console_count(void);
const Console* get_console(size_t index);
bool is_console_registered(const Console* console);

bool activate_console(const Console* console);
bool deactivate_console(void);
const Console* get_active_console(void);

size_t read_from_console(char* data, size_t size);
size_t write_to_console(const char* data);
bool put_to_console(char chr);

size_t vcprintf(const char* restrict format, va_list arg);
size_t cprintf(const char* restrict format, ...);

#endif  /* KERNEL_CONSOLE_H */
