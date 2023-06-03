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

#include <kernel/device/ns16550a/ns16550a.h>

#include <kernel/config.h>
#include <kernel/console.h>
#include <kernel/device.h>

#include <string.h>

const Ns16550aUart ns16550a_uart0 = {
    .base = (uint8_t*)UART0_BASE,
    .size = (size_t)UART0_SIZE,
    .register_width = UART0_REGISTER_WIDTH,
};

static volatile uint8_t* _get_register_address(const Ns16550aUart* uart,
    uint8_t index)
{
    return uart->base + index * uart->register_width;
}

uint8_t ns16550a_read_register(const Ns16550aUart* uart, uint8_t index)
{
    return *_get_register_address(uart, index);
}

void ns16550a_write_register(const Ns16550aUart* uart, uint8_t index,
    uint8_t value)
{
    *_get_register_address(uart, index) = value;
}

bool ns16550a_try_receive(const Ns16550aUart* uart, uint8_t* chr)
{
    if (NS16550A_READ_FIELD(uart, LSR, RBR)) {
        *chr = NS16550A_READ_FIELD(uart, RBR, CHR);
        return true;
    }
    return false;
}

size_t ns16550a_receive_buffer(const Ns16550aUart* uart, uint8_t* data,
    size_t size)
{
    if (uart == NULL || data == NULL) {
        return 0;
    }

    uint8_t chr;
    size_t i = 0;
    while (i < size && ns16550a_try_receive(uart, &chr)) {
        data[i] = chr;
        ++i;
    }
    return i;
}

bool ns16550a_transmit(const Ns16550aUart* uart, uint8_t chr)
{
    if (NS16550A_READ_FIELD(uart, LSR, THR)) {
        NS16550A_WRITE_FIELD(uart, THR, CHR, chr);
        return true;
    }
    return false;
}

size_t ns16550a_transmit_buffer(const Ns16550aUart* uart, const uint8_t* data,
    size_t size)
{
    if (uart == NULL || data == NULL) {
        return 0;
    }

    size_t i = 0;
    for (; i < size; ++i) {
        if (!ns16550a_transmit(uart, data[i])) {
            return size;
        }
    }
    return size;
}

static void _activate_console(const Console* console)
{
    (void)console;
}

static void _deactivate_console(const Console* console)
{
    (void)console;
}

static size_t _read_from_console(const Console* console, char* data,
    size_t size)
{
    return ns16550a_receive_buffer(console->tag, (uint8_t*)data, size);
}

static size_t _write_to_console(const Console* console, const char* data)
{
    return ns16550a_transmit_buffer(console->tag, (const uint8_t*)data,
        strlen(data));
}

static bool _put_to_console(const Console* console, char chr)
{
    return ns16550a_transmit(console->tag, chr);
}

static const Console _uart0_console = {
    .name = "uart0",
    .tag = &ns16550a_uart0,
    .activate = _activate_console,
    .deactivate = _deactivate_console,
    .read = _read_from_console,
    .write = _write_to_console,
    .put = _put_to_console,
};

void ns16550a_initialize(void)
{
    register_console(&_uart0_console);
}

void ns16550a_finalize(void)
{
    deregister_console(&_uart0_console);
}

DEVICE_INITIALIZER(ns16550a, ns16550a_initialize);
DEVICE_FINALIZER(ns16550a, ns16550a_finalize);
