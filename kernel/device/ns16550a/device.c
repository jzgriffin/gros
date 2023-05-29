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

#include <kernel/config.h>
#include <kernel/console.h>
#include <kernel/device.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Receiver buffer register
#define RBR_INDEX      0x00
#define RBR_CHR_OFFSET 0  // Character
#define RBR_CHR_MASK   (0xFF << RBR_CHR_OFFSET)

// Transmitter holding register
#define THR_INDEX      0x00
#define THR_CHR_OFFSET 0  // Character
#define THR_CHR_MASK   (0xFF << THR_CHR_OFFSET)

// Interrupt enable register
#define IER_INDEX      0x01
#define IER_RBR_OFFSET 0  // RBR non-empty
#define IER_RBR_MASK   (0x01 << IER_RBR_OFFSET)
#define IER_THR_OFFSET 1  // THR empty
#define IER_THR_MASK   (0x01 << IER_THR_OFFSET)
#define IER_LSR_OFFSET 2  // Line status
#define IER_LSR_MASK   (0x01 << IER_LSR_OFFSET)
#define IER_MSR_OFFSET 3  // Modem status
#define IER_MSR_MASK   (0x01 << IER_MSR_OFFSET)
#define IER_RXE_OFFSET 6  // DMA RX end
#define IER_RXE_MASK   (0x01 << IER_RXE_OFFSET)
#define IER_TXE_OFFSET 7  // DMA TX end
#define IER_TXE_MASK   (0x01 << IER_TXE_OFFSET)

// Interrupt status register
#define ISR_INDEX      0x02
#define ISR_NIP_OFFSET 0  // Interrupt status (no interrupt pending)
#define ISR_NIP_MASK   (0x01 << ISR_NIP_OFFSET)
#define ISR_IIC_OFFSET 1  // Interrupt identification code
#define ISR_IIC_MASK   (0x07 << ISR_IIC_OFFSET)
#define ISR_IIC_CLR    0x00  // No interrupt
#define ISR_IIC_LSR    0x03  // LSR change
#define ISR_IIC_RHR    0x02  // RHR non-empty
#define ISR_IIC_RTO    0x06  // RX timeout
#define ISR_IIC_THR    0x01  // THR empty
#define ISR_IIC_MSR    0x00  // MSR change
#define ISR_IIC_RXE    0x07  // DMA RX end
#define ISR_IIC_TXE    0x05  // DMA TX end
#define ISR_RXE_OFFSET 4  // DMA RX end
#define ISR_RXE_MASK   (0x01 << ISR_RXE_OFFSET)
#define ISR_TXE_OFFSET 5  // DMA TX end
#define ISR_TXE_MASK   (0x01 << ISR_TXE_OFFSET)
#define ISR_FE0_OFFSET 6  // FIFOs enabled
#define ISR_FE0_MASK   (0x01 << ISR_FE0_OFFSET)
#define ISR_FE1_OFFSET 7  // FIFOs enabled
#define ISR_FE1_MASK   (0x01 << ISR_FE1_OFFSET)

// FIFO control register
#define FCR_INDEX      0x02
#define FCR_FEN_OFFSET 0  // FIFO enable
#define FCR_FEN_MASK   (0x01 << FCR_FEN_OFFSET)
#define FCR_RXR_OFFSET 1  // RX FIFO reset
#define FCR_RXR_MASK   (0x01 << FCR_RXR_OFFSET)
#define FCR_TXR_OFFSET 2  // TX FIFO reset
#define FCR_TXR_MASK   (0x01 << FCR_TXR_OFFSET)
#define FCR_DMA_OFFSET 3  // DMA mode
#define FCR_DMA_MASK   (0x01 << FCR_DMA_OFFSET)
#define FCR_EDE_OFFSET 4  // Enable DMA end
#define FCR_EDE_MASK   (0x01 << FCR_EDE_OFFSET)
#define FCR_RTL_OFFSET 6  // RX FIFO trigger level
#define FCR_RTL_MASK   (0x03 << FCR_RTL_OFFSET)
#define FCR_RTL_1CHR   0x00  // 1 character FIFO
#define FCR_RTL_4CHR   0x01  // 4 character FIFO
#define FCR_RTL_8CHR   0x02  // 8 character FIFO
#define FCR_RTL_14CH   0x03  // 14 character FIFO

// Line control register
#define LCR_INDEX      0x03
#define LCR_WLN_OFFSET 0  // Word length
#define LCR_WLN_MASK   (0x03 << LCR_WLN_OFFSET)
#define LCR_WLN_5BIT   0x00  // 5-bit word length
#define LCR_WLN_6BIT   0x01  // 6-bit word length
#define LCR_WLN_7BIT   0x02  // 7-bit word length
#define LCR_WLN_8BIT   0x03  // 8-bit word length
#define LCR_SBS_OFFSET 2  // Stop bits
#define LCR_SBS_MASK   (0x01 << LCR_SBS_OFFSET)
#define LCR_PEN_OFFSET 3  // Parity enable
#define LCR_PEN_MASK   (0x01 << LCR_PEN_OFFSET)
#define LCR_EVP_OFFSET 4  // Even parity
#define LCR_EVP_MASK   (0x01 << LCR_EVP_OFFSET)
#define LCR_FOP_OFFSET 5  // Force parity
#define LCR_FOP_MASK   (0x01 << LCR_FOP_OFFSET)
#define LCR_SBK_OFFSET 6  // Set break
#define LCR_SBK_MASK   (0x01 << LCR_SBK_OFFSET)
#define LCR_DLA_OFFSET 7  // Divisor latch access
#define LCR_DLA_MASK   (0x01 << LCR_DLA_OFFSET)

// Modem control register
#define MCR_INDEX      0x04
#define MCR_DTR_OFFSET 0  // DTR
#define MCR_DTR_MASK   (0x01 << MCR_DTR_OFFSET)
#define MCR_RCS_OFFSET 1  // RCS
#define MCR_RCS_MASK   (0x01 << MCR_RCS_OFFSET)
#define MCR_OU1_OFFSET 2  // Output 1
#define MCR_OU1_MASK   (0x01 << MCR_OU1_OFFSET)
#define MCR_OU2_OFFSET 3  // Output 2 / int. enable
#define MCR_OU2_MASK   (0x01 << MCR_OU2_OFFSET)
#define MCR_LBK_OFFSET 4  // Loop back
#define MCR_LBK_MASK   (0x01 << MCR_LBK_OFFSET)

// Line status register
#define LSR_INDEX      0x05
#define LSR_RBR_OFFSET 0  // RBR non-empty
#define LSR_RBR_MASK   (0x01 << LSR_RBR_OFFSET)
#define LSR_OER_OFFSET 1  // Overrun error
#define LSR_OER_MASK   (0x01 << LSR_OER_OFFSET)
#define LSR_PER_OFFSET 2  // Parity error
#define LSR_PER_MASK   (0x01 << LSR_PER_OFFSET)
#define LSR_FER_OFFSET 3  // Framing error
#define LSR_FER_MASK   (0x01 << LSR_FER_OFFSET)
#define LSR_BKI_OFFSET 4  // Break interrupt
#define LSR_BKI_MASK   (0x01 << LSR_BKI_OFFSET)
#define LSR_THR_OFFSET 5  // THR empty
#define LSR_THR_MASK   (0x01 << LSR_THR_OFFSET)
#define LSR_TXE_OFFSET 6  // Transmitter empty
#define LSR_TXE_MASK   (0x01 << LSR_TXE_OFFSET)
#define LSR_FDE_OFFSET 7  // FIFO data error
#define LSR_FDE_MASK   (0x01 << LSR_FDE_OFFSET)

// Modem status register
#define MSR_INDEX       0x06
#define MSR_DCTS_OFFSET 0  // Delta clear to send
#define MSR_DCTS_MASK   (0x01 << MSR_DCTS_OFFSET)
#define MSR_DDSR_OFFSET 1  // Delta data set ready
#define MSR_DDSR_MASK   (0x01 << MSR_DDSR_OFFSET)
#define MSR_TERI_OFFSET 2  // Trailing edge ring indicator
#define MSR_TERI_MASK   (0x01 << MSR_TERI_OFFSET)
#define MSR_DCD_OFFSET  3  // Delta carrier detect
#define MSR_DCD_MASK    (0x01 << MSR_DCD_OFFSET)
#define MSR_CTS_OFFSET  4  // Clear to send
#define MSR_CTS_MASK    (0x01 << MSR_CTS_OFFSET)
#define MSR_DSR_OFFSET  5  // Data set ready
#define MSR_DSR_MASK    (0x01 << MSR_DSR_OFFSET)
#define MSR_RI_OFFSET   6  // Ring indicator
#define MSR_RI_MASK     (0x01 << MSR_RI_OFFSET)
#define MSR_CD_OFFSET   7  // Carrier detect
#define MSR_CD_MASK     (0x01 << MSR_CD_OFFSET)

// Scratch pad register
#define SPR_INDEX      0x07
#define SPR_UDA_OFFSET 0  // User data
#define SPR_UDA_MASK   (0xFF << SPR_UDA_OFFSET)

// Divisor latch LSB
#define DLL_INDEX      0x00
#define DLL_BDL_OFFSET 0  // Baudrate divisor constant LSB
#define DLL_BDL_MASK   (0xFF << DLL_BDL_OFFSET)

// Divisor latch MSB
#define DLM_INDEX      0x00
#define DLM_BDM_OFFSET 0  // Baudrate divisor constant MSB
#define DLM_BDM_MASK   (0xFF << DLM_BDM_OFFSET)

// Prescaler division
#define PSD_INDEX      0x05
#define PSD_PDF_OFFSET 0  // Prescaler division factor
#define PSD_PDF_MASK   (0x0F << PSD_PDF_OFFSET)

// Field accessors
#define READ_FIELD(uart, reg, field) \
    ((read_register(uart, (reg ## _INDEX)) & \
            (reg ## _ ## field ## _MASK)) >> \
        (reg ## _ ## field ## _OFFSET))
#define WRITE_FIELD(uart, reg, field, value) \
    (write_register(uart, (reg ## _INDEX), \
        (READ_FIELD(uart, reg, field) & \
             ~(reg ## _ ## field ## _MASK)) | \
        (((value) << (reg ## _ ## field ## _OFFSET)) & \
            (reg ## _ ## field ## _MASK))))

typedef struct
{
    volatile uint8_t* base;
    size_t size;
    size_t register_width;
} Uart;

static const Uart uart0 = {
    .base = (uint8_t*)UART0_BASE,
    .size = (size_t)UART0_SIZE,
    .register_width = UART0_REGISTER_WIDTH,
};

static volatile uint8_t* get_register_address(const Uart* uart, uint8_t index)
{
    return uart->base + index * uart->register_width;
}

static uint8_t read_register(const Uart* uart, uint8_t index)
{
    return *get_register_address(uart, index);
}

static void write_register(const Uart* uart, uint8_t index, uint8_t value)
{
    *get_register_address(uart, index) = value;
}

static bool try_receive(const Uart* uart, uint8_t* chr)
{
    if (READ_FIELD(uart, LSR, RBR)) {
        *chr = READ_FIELD(uart, RBR, CHR);
        return true;
    }
    return false;
}

static size_t receive_buffer(const Uart* uart, uint8_t* data, size_t size)
{
    if (uart == NULL || data == NULL) {
        return 0;
    }

    uint8_t chr;
    size_t i = 0;
    while (i < size && try_receive(uart, &chr)) {
        data[i] = chr;
        ++i;
    }
    return i;
}

static bool transmit(const Uart* uart, uint8_t chr)
{
    if (READ_FIELD(uart, LSR, THR)) {
        WRITE_FIELD(uart, THR, CHR, chr);
        return true;
    }
    return false;
}

static size_t transmit_buffer(const Uart* uart, const uint8_t* data,
        size_t size)
{
    if (uart == NULL || data == NULL) {
        return 0;
    }

    size_t i = 0;
    for (; i < size; ++i) {
        if (!transmit(uart, data[i])) {
            return size;
        }
    }
    return size;
}

static void console_activate(const Console* console)
{
    (void)console;
}

static void console_deactivate(const Console* console)
{
    (void)console;
}

static size_t console_read(const Console* console, char* data, size_t size)
{
    return receive_buffer(console->tag, (uint8_t*)data, size);
}

static size_t console_write(const Console* console, const char* data)
{
    return transmit_buffer(console->tag, (const uint8_t*)data, strlen(data));
}

static bool console_put(const Console* console, char chr)
{
    return transmit(console->tag, chr);
}

static const Console uart0_console = {
    .name = "uart0",
    .tag = &uart0,
    .activate = console_activate,
    .deactivate = console_deactivate,
    .read = console_read,
    .write = console_write,
    .put = console_put,
};

static void initialize(void)
{
    register_console(&uart0_console);
}

static void finalize(void)
{
    deregister_console(&uart0_console);
}

DEVICE_INITIALIZER(ns16550a, initialize);
DEVICE_FINALIZER(ns16550a, finalize);
