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
