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

#include <kernel/console.h>

#include <string.h>

#define MAX_CONSOLES 8

static const Console* _consoles[MAX_CONSOLES] = {NULL};
static size_t _console_count = 0;
static const Console* _active_console = NULL;

bool register_console(const Console* console)
{
    if (console == NULL) {
        return false;
    }
    if (_console_count >= MAX_CONSOLES) {
        return false;
    }

    for (size_t i = 0; i < _console_count; ++i) {
        if (_consoles[i] == console) {
            return false;
        }
    }

    _consoles[_console_count] = console;
    ++_console_count;
    if (_console_count == 1) {
        activate_console(console);
    }
    return true;
}

bool deregister_console(const Console* console)
{
    if (console == NULL) {
        return false;
    }

    bool was_removed = false;
    for (size_t i = 0; i < _console_count; ++i) {
        if (_consoles[i] == console) {
            deactivate_console();
            was_removed = true;
        }
        if (was_removed) {
            if (i + 1 < _console_count) {
                _consoles[i] = _consoles[i + 1];
            }
            else {
                _consoles[i] = NULL;
            }
        }
    }
    return was_removed;
}

size_t get_console_count(void)
{
    return _console_count;
}

const Console* get_console(size_t index)
{
    return index < _console_count ? _consoles[index] : NULL;
}

bool is_console_registered(const Console *console)
{
    if (console == NULL) {
        return false;
    }

    for (size_t i = 0; i < _console_count; ++i) {
        if (_consoles[i] == console) {
            return true;
        }
    }
    return false;
}

bool activate_console(const Console* console)
{
    if (!is_console_registered(console)) {
        return false;
    }

    if (_active_console != NULL) {
        _active_console->deactivate(_active_console);
    }
    _active_console = console;
    _active_console->activate(_active_console);
    return true;
}

bool deactivate_console(void)
{
    if (_active_console == NULL) {
        return false;
    }

    _active_console->deactivate(_active_console);
    _active_console = NULL;
    return true;
}

const Console* get_active_console(void)
{
    return _active_console;
}

size_t read_from_console(char* data, size_t size)
{
    if (_active_console == NULL) {
        return 0;
    }

    return _active_console->read(_active_console, data, size);
}

size_t write_to_console(const char* data)
{
    if (_active_console == NULL) {
        return 0;
    }

    return _active_console->write(_active_console, data);
}

bool put_to_console(char chr)
{
    if (_active_console == NULL) {
        return 0;
    }

    return _active_console->put(_active_console, chr);
}

#define INT_BUFFER_SIZE  48  /* Large enough to contain 2^128 plus sign. */
#define UINT_BUFFER_SIZE 48  /* Large enough to contain 2^128. */

static char* _convert_int(char* buffer, intmax_t i, size_t base,
        bool uppercase)
{
    static char lowercase_chars[] = "0123456789abcdef";
    static char uppercase_chars[] = "0123456789ABCDEF";

    const char* chars = uppercase ? uppercase_chars : lowercase_chars;

    const bool negative = i < 0;
    if (negative) {
        i *= -1;
    }

    char* ptr = &buffer[INT_BUFFER_SIZE - 1];
    *ptr = '\0';
    do {
        --ptr;
        *ptr = chars[i % base];
        i /= base;
    } while (i != 0);
    if (negative) {
        --ptr;
        *ptr = '-';
    }
    return ptr;
}

static char* _convert_uint(char* buffer, uintmax_t i, size_t base,
        bool uppercase)
{
    static char lowercase_chars[] = "0123456789abcdef";
    static char uppercase_chars[] = "0123456789ABCDEF";

    const char* chars = uppercase ? uppercase_chars : lowercase_chars;

    char* ptr = &buffer[UINT_BUFFER_SIZE - 1];
    *ptr = '\0';
    do {
        --ptr;
        *ptr = chars[i % base];
        i /= base;
    } while (i != 0);
    return ptr;
}

size_t vcprintf(const char* restrict format, va_list arg)
{
    if (format == NULL) {
        return 0;
    }

    size_t size = 0;
    while (*format != '\0') {
        if (*format != '%') {
            if (put_to_console(*format)) {
                ++format;
                ++size;
            }
            else {
                return size;
            }
            continue;
        }

        ++format;
        switch (*format) {
            case '%':
                if (put_to_console('%')) {
                    ++format;
                    ++size;
                }
                else {
                    return size;
                }
                break;

            case 'c':
                if (put_to_console((char)va_arg(arg, int))) {
                    ++format;
                    ++size;
                }
                else {
                    return size;
                }
                break;

            case 's': {
                const char* s = va_arg(arg, const char*);
                const size_t n = write_to_console(s);
                size += n;
                if (n == strlen(s)) {
                    ++format;
                }
                else {
                    return size;
                }
                break;
            }

            case 'd':  // Fallthrough
            case 'i': {
                char buffer[INT_BUFFER_SIZE];
                const char* s =
                    _convert_int(buffer, va_arg(arg, int), 10, false);
                const size_t n = write_to_console(s);
                size += n;
                if (n == strlen(s)) {
                    ++format;
                }
                else {
                    return size;
                }
                break;
            }

            case 'o': {
                char buffer[INT_BUFFER_SIZE];
                const char* s =
                    _convert_uint(buffer, va_arg(arg, int), 8, false);
                const size_t n = write_to_console(s);
                size += n;
                if (n == strlen(s)) {
                    ++format;
                }
                else {
                    return size;
                }
                break;
            }

            case 'x':
            case 'X': {  // Fallthrough
                char buffer[UINT_BUFFER_SIZE];
                const char* s = _convert_uint(buffer, va_arg(arg, int), 16,
                        *format == 'X');
                const size_t n = write_to_console(s);
                size += n;
                if (n == strlen(s)) {
                    ++format;
                }
                else {
                    return size;
                }
                break;
            }

            case 'u': {
                char buffer[UINT_BUFFER_SIZE];
                const char* s =
                    _convert_uint(buffer, va_arg(arg, int), 10, false);
                const size_t n = write_to_console(s);
                size += n;
                if (n == strlen(s)) {
                    ++format;
                }
                else {
                    return size;
                }
                break;
            }

            case 'n': {
                char buffer[UINT_BUFFER_SIZE];
                const char* s = _convert_uint(buffer, size, 10, false);
                const size_t n = write_to_console(s);
                size += n;
                if (n == strlen(s)) {
                    ++format;
                }
                else {
                    return size;
                }
                break;
            }

            case 'p': {
                void* ptr = va_arg(arg, void*);
                if (ptr == NULL) {
                    static const char s[] = "(nil)";
                    const size_t n = write_to_console(s);
                    size += n;
                    if (n == strlen(s)) {
                        ++format;
                    }
                    else {
                        return size;
                    }
                }
                else {
                    if (put_to_console('0')) {
                        ++size;
                    }
                    else {
                        return size;
                    }
                    if (put_to_console('x')) {
                        ++size;
                    }
                    else {
                        return size;
                    }
                    char buffer[UINT_BUFFER_SIZE];
                    const char* s = _convert_uint(buffer, size, 16, true);
                    const size_t n = write_to_console(s);
                    if (n == strlen(s)) {
                        ++format;
                        size += n;
                    }
                    else {
                        return size;
                    }
                }
                break;
            }
        }
    }
    return size;
}

size_t cprintf(const char* restrict format, ...)
{
    va_list arg;
    va_start(arg, format);
    const size_t count = vcprintf(format, arg);
    va_end(arg);
    return count;
}
