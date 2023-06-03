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

#include <stdio.h>

#include <kernel/console.h>
#include <kernel/debug.h>

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum PrintType
{
    print_type_measure,
    print_type_debug,
    print_type_console,
    print_type_string,
} PrintType;

typedef struct PrintState
{
    const char* format;
    va_list arg;
    size_t capacity;
    size_t length;
    PrintType type;
    union {
        char* string;
    } output;

    // Formatting flags.
    size_t base;
    bool lowercase;
} PrintState;

static void _reset_print_flags(PrintState* state)
{
    state->base = 10;
    state->lowercase = false;
}

static void _finalize_print(PrintState* state)
{
    va_end(state->arg);

    switch (state->type) {
        case print_type_measure:
            break;

        case print_type_debug:
            break;

        case print_type_console:
            break;

        case print_type_string:
            state->output.string[state->length] = '\0';
            break;
    }
}

static void _do_print_chr(PrintState* state, char c)
{
    if (state->length + 1 < state->capacity) {
        switch (state->type) {
            case print_type_measure:
                break;

            case print_type_debug:
                dputc(c);
                break;

            case print_type_console:
                put_to_console(c);
                break;

            case print_type_string:
                state->output.string[state->length] = c;
                break;
        }
    }
    ++state->length;
}

static void _do_print_str(PrintState* state, const char* s)
{
    for (; *s != '\0'; ++s) {
        _do_print_chr(state, *s);
    }
}

static void _do_print_int(PrintState* state, intmax_t i)
{
    static const char* digits = "0123456789";
    static const size_t buffer_size = 48;

    const bool negative = i < 0;
    if (negative) {
        i *= -1;
    }

    char s[buffer_size];
    char* p = &s[buffer_size - 1];
    *p = '\0';
    do {
        --p;
        *p = digits[i % state->base];
        i /= state->base;
    } while (i != 0);

    if (negative) {
        --p;
        *p = '-';
    }

    _do_print_str(state, p);
}

static void _do_print_uint(PrintState* state, uintmax_t i)
{
    static const char* lowercase_digits = "0123456789abcdef";
    static const char* uppercase_digits = "0123456789ABCDEF";
    static const size_t buffer_size = 48;

    const char* digits = state->lowercase
        ? lowercase_digits
        : uppercase_digits;

    char s[buffer_size];
    char* p = &s[buffer_size - 1];
    *p = '\0';
    do {
        --p;
        *p = digits[i % state->base];
        i /= state->base;
    } while (i != 0);

    _do_print_str(state, p);
}

static void _do_print_ptr(PrintState* state, void* p)
{
    _reset_print_flags(state);  // Do not apply any user-specified flags.

    if (p == NULL) {
        _do_print_str(state, "(nil)");
    }
    else {
        _do_print_str(state, "0x");
        state->base = 16;
        _do_print_uint(state, (uintptr_t)p);
    }
}

static int _do_print(PrintState* state)
{
    while (*state->format != '\0') {
        if (*state->format != '%') {
            _do_print_chr(state, *state->format);
            ++state->format;
            continue;
        }

        _reset_print_flags(state);

        ++state->format;
        switch (*state->format) {
            case '%':
                _do_print_chr(state, *state->format);
                ++state->format;
                break;

            case 'c':
                _do_print_chr(state, (unsigned char)va_arg(state->arg, int));
                ++state->format;
                break;

            case 's':
                _do_print_str(state, va_arg(state->arg, const char*));
                ++state->format;
                break;

            case 'd':  // Fallthrough
            case 'i':
                _do_print_int(state, va_arg(state->arg, int));
                ++state->format;
                break;

            case 'o':
                state->base = 8;
                _do_print_uint(state, va_arg(state->arg, unsigned int));
                ++state->format;
                break;

            case 'x':
                state->base = 16;
                state->lowercase = true;
                _do_print_uint(state, va_arg(state->arg, unsigned int));
                ++state->format;
                break;

            case 'X':
                state->base = 16;
                _do_print_uint(state, va_arg(state->arg, unsigned int));
                ++state->format;
                break;

            case 'u':
                _do_print_uint(state, va_arg(state->arg, unsigned int));
                ++state->format;
                break;

            case 'n':
                *va_arg(state->arg, int*) = state->length;
                ++state->format;
                break;

            case 'p':
                _do_print_ptr(state, va_arg(state->arg, void*));
                ++state->format;
                break;

            default:
                _finalize_print(state);
                return EOF;
        }
    }
    _finalize_print(state);
    return (int)state->length;
}

int snprintf(char* restrict s, size_t n, const char* restrict format, ...)
{
    va_list arg;
    va_start(arg, format);
    int length = vsnprintf(s, n, format, arg);
    va_end(arg);
    return length;
}

int vsnprintf(char* restrict s, size_t n, const char* restrict format,
    va_list arg)
{
    if (format == NULL) {
        return 0;
    }

    // The length of a fully-filled string must be within the range of this
    // function's return type.
    if (n > INT_MAX) {
        return EOF;
    }

    PrintState state = {
        .format = format,
        .capacity = n,
        .length = 0,
        .type = s == NULL ? print_type_measure : print_type_string,
        .output.string = s,
    };
    va_copy(state.arg, arg);
    return _do_print(&state);
}

int printf(const char* restrict format, ...)
{
    va_list arg;
    va_start(arg, format);
    int length = vprintf(format, arg);
    va_end(arg);
    return length;
}

int vprintf(const char* restrict format, va_list arg)
{
    if (format == NULL) {
        return 0;
    }

    PrintState state = {
        .format = format,
        .capacity = INT_MAX,
        .length = 0,
        .type = print_type_console,
    };
    va_copy(state.arg, arg);
    return _do_print(&state);
}

int dprintf(const char* restrict format, ...)
{
    va_list arg;
    va_start(arg, format);
    int length = vdprintf(format, arg);
    va_end(arg);
    return length;
}

int vdprintf(const char* restrict format, va_list arg)
{
    if (format == NULL) {
        return 0;
    }

    PrintState state = {
        .format = format,
        .capacity = INT_MAX,
        .length = 0,
        .type = print_type_debug,
    };
    va_copy(state.arg, arg);
    return _do_print(&state);
}
