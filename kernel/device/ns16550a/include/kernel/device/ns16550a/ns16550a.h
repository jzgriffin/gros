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

#ifndef KERNEL_DEVICE_NS16550A_NS16550A_H
#define KERNEL_DEVICE_NS16550A_NS16550A_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Receiver buffer register
#define NS16550A_RBR_INDEX      0x00
#define NS16550A_RBR_CHR_OFFSET 0  // Character
#define NS16550A_RBR_CHR_MASK   (0xFF << NS16550A_RBR_CHR_OFFSET)

// Transmitter holding register
#define NS16550A_THR_INDEX      0x00
#define NS16550A_THR_CHR_OFFSET 0  // Character
#define NS16550A_THR_CHR_MASK   (0xFF << NS16550A_THR_CHR_OFFSET)

// Interrupt enable register
#define NS16550A_IER_INDEX      0x01
#define NS16550A_IER_RBR_OFFSET 0  // RBR non-empty
#define NS16550A_IER_RBR_MASK   (0x01 << NS16550A_IER_RBR_OFFSET)
#define NS16550A_IER_THR_OFFSET 1  // THR empty
#define NS16550A_IER_THR_MASK   (0x01 << NS16550A_IER_THR_OFFSET)
#define NS16550A_IER_LSR_OFFSET 2  // Line status
#define NS16550A_IER_LSR_MASK   (0x01 << NS16550A_IER_LSR_OFFSET)
#define NS16550A_IER_MSR_OFFSET 3  // Modem status
#define NS16550A_IER_MSR_MASK   (0x01 << NS16550A_IER_MSR_OFFSET)
#define NS16550A_IER_RXE_OFFSET 6  // DMA RX end
#define NS16550A_IER_RXE_MASK   (0x01 << NS16550A_IER_RXE_OFFSET)
#define NS16550A_IER_TXE_OFFSET 7  // DMA TX end
#define NS16550A_IER_TXE_MASK   (0x01 << NS16550A_IER_TXE_OFFSET)

// Interrupt status register
#define NS16550A_ISR_INDEX      0x02
#define NS16550A_ISR_NIP_OFFSET 0  // Interrupt status (no interrupt pending)
#define NS16550A_ISR_NIP_MASK   (0x01 << NS16550A_ISR_NIP_OFFSET)
#define NS16550A_ISR_IIC_OFFSET 1  // Interrupt identification code
#define NS16550A_ISR_IIC_MASK   (0x07 << NS16550A_ISR_IIC_OFFSET)
#define NS16550A_ISR_IIC_CLR    0x00  // No interrupt
#define NS16550A_ISR_IIC_LSR    0x03  // LSR change
#define NS16550A_ISR_IIC_RHR    0x02  // RHR non-empty
#define NS16550A_ISR_IIC_RTO    0x06  // RX timeout
#define NS16550A_ISR_IIC_THR    0x01  // THR empty
#define NS16550A_ISR_IIC_MSR    0x00  // MSR change
#define NS16550A_ISR_IIC_RXE    0x07  // DMA RX end
#define NS16550A_ISR_IIC_TXE    0x05  // DMA TX end
#define NS16550A_ISR_RXE_OFFSET 4  // DMA RX end
#define NS16550A_ISR_RXE_MASK   (0x01 << NS16550A_ISR_RXE_OFFSET)
#define NS16550A_ISR_TXE_OFFSET 5  // DMA TX end
#define NS16550A_ISR_TXE_MASK   (0x01 << NS16550A_ISR_TXE_OFFSET)
#define NS16550A_ISR_FE0_OFFSET 6  // FIFOs enabled
#define NS16550A_ISR_FE0_MASK   (0x01 << NS16550A_ISR_FE0_OFFSET)
#define NS16550A_ISR_FE1_OFFSET 7  // FIFOs enabled
#define NS16550A_ISR_FE1_MASK   (0x01 << NS16550A_ISR_FE1_OFFSET)

// FIFO control register
#define NS16550A_FCR_INDEX      0x02
#define NS16550A_FCR_FEN_OFFSET 0  // FIFO enable
#define NS16550A_FCR_FEN_MASK   (0x01 << NS16550A_FCR_FEN_OFFSET)
#define NS16550A_FCR_RXR_OFFSET 1  // RX FIFO reset
#define NS16550A_FCR_RXR_MASK   (0x01 << NS16550A_FCR_RXR_OFFSET)
#define NS16550A_FCR_TXR_OFFSET 2  // TX FIFO reset
#define NS16550A_FCR_TXR_MASK   (0x01 << NS16550A_FCR_TXR_OFFSET)
#define NS16550A_FCR_DMA_OFFSET 3  // DMA mode
#define NS16550A_FCR_DMA_MASK   (0x01 << NS16550A_FCR_DMA_OFFSET)
#define NS16550A_FCR_EDE_OFFSET 4  // Enable DMA end
#define NS16550A_FCR_EDE_MASK   (0x01 << NS16550A_FCR_EDE_OFFSET)
#define NS16550A_FCR_RTL_OFFSET 6  // RX FIFO trigger level
#define NS16550A_FCR_RTL_MASK   (0x03 << NS16550A_FCR_RTL_OFFSET)
#define NS16550A_FCR_RTL_1CHR   0x00  // 1 character FIFO
#define NS16550A_FCR_RTL_4CHR   0x01  // 4 character FIFO
#define NS16550A_FCR_RTL_8CHR   0x02  // 8 character FIFO
#define NS16550A_FCR_RTL_14CH   0x03  // 14 character FIFO

// Line control register
#define NS16550A_LCR_INDEX      0x03
#define NS16550A_LCR_WLN_OFFSET 0  // Word length
#define NS16550A_LCR_WLN_MASK   (0x03 << NS16550A_LCR_WLN_OFFSET)
#define NS16550A_LCR_WLN_5BIT   0x00  // 5-bit word length
#define NS16550A_LCR_WLN_6BIT   0x01  // 6-bit word length
#define NS16550A_LCR_WLN_7BIT   0x02  // 7-bit word length
#define NS16550A_LCR_WLN_8BIT   0x03  // 8-bit word length
#define NS16550A_LCR_SBS_OFFSET 2  // Stop bits
#define NS16550A_LCR_SBS_MASK   (0x01 << NS16550A_LCR_SBS_OFFSET)
#define NS16550A_LCR_PEN_OFFSET 3  // Parity enable
#define NS16550A_LCR_PEN_MASK   (0x01 << NS16550A_LCR_PEN_OFFSET)
#define NS16550A_LCR_EVP_OFFSET 4  // Even parity
#define NS16550A_LCR_EVP_MASK   (0x01 << NS16550A_LCR_EVP_OFFSET)
#define NS16550A_LCR_FOP_OFFSET 5  // Force parity
#define NS16550A_LCR_FOP_MASK   (0x01 << NS16550A_LCR_FOP_OFFSET)
#define NS16550A_LCR_SBK_OFFSET 6  // Set break
#define NS16550A_LCR_SBK_MASK   (0x01 << NS16550A_LCR_SBK_OFFSET)
#define NS16550A_LCR_DLA_OFFSET 7  // Divisor latch access
#define NS16550A_LCR_DLA_MASK   (0x01 << NS16550A_LCR_DLA_OFFSET)

// Modem control register
#define NS16550A_MCR_INDEX      0x04
#define NS16550A_MCR_DTR_OFFSET 0  // DTR
#define NS16550A_MCR_DTR_MASK   (0x01 << NS16550A_MCR_DTR_OFFSET)
#define NS16550A_MCR_RCS_OFFSET 1  // RCS
#define NS16550A_MCR_RCS_MASK   (0x01 << NS16550A_MCR_RCS_OFFSET)
#define NS16550A_MCR_OU1_OFFSET 2  // Output 1
#define NS16550A_MCR_OU1_MASK   (0x01 << NS16550A_MCR_OU1_OFFSET)
#define NS16550A_MCR_OU2_OFFSET 3  // Output 2 / int. enable
#define NS16550A_MCR_OU2_MASK   (0x01 << NS16550A_MCR_OU2_OFFSET)
#define NS16550A_MCR_LBK_OFFSET 4  // Loop back
#define NS16550A_MCR_LBK_MASK   (0x01 << NS16550A_MCR_LBK_OFFSET)

// Line status register
#define NS16550A_LSR_INDEX      0x05
#define NS16550A_LSR_RBR_OFFSET 0  // RBR non-empty
#define NS16550A_LSR_RBR_MASK   (0x01 << NS16550A_LSR_RBR_OFFSET)
#define NS16550A_LSR_OER_OFFSET 1  // Overrun error
#define NS16550A_LSR_OER_MASK   (0x01 << NS16550A_LSR_OER_OFFSET)
#define NS16550A_LSR_PER_OFFSET 2  // Parity error
#define NS16550A_LSR_PER_MASK   (0x01 << NS16550A_LSR_PER_OFFSET)
#define NS16550A_LSR_FER_OFFSET 3  // Framing error
#define NS16550A_LSR_FER_MASK   (0x01 << NS16550A_LSR_FER_OFFSET)
#define NS16550A_LSR_BKI_OFFSET 4  // Break interrupt
#define NS16550A_LSR_BKI_MASK   (0x01 << NS16550A_LSR_BKI_OFFSET)
#define NS16550A_LSR_THR_OFFSET 5  // THR empty
#define NS16550A_LSR_THR_MASK   (0x01 << NS16550A_LSR_THR_OFFSET)
#define NS16550A_LSR_TXE_OFFSET 6  // Transmitter empty
#define NS16550A_LSR_TXE_MASK   (0x01 << NS16550A_LSR_TXE_OFFSET)
#define NS16550A_LSR_FDE_OFFSET 7  // FIFO data error
#define NS16550A_LSR_FDE_MASK   (0x01 << NS16550A_LSR_FDE_OFFSET)

// Modem status register
#define NS16550A_MSR_INDEX       0x06
#define NS16550A_MSR_DCTS_OFFSET 0  // Delta clear to send
#define NS16550A_MSR_DCTS_MASK   (0x01 << NS16550A_MSR_DCTS_OFFSET)
#define NS16550A_MSR_DDSR_OFFSET 1  // Delta data set ready
#define NS16550A_MSR_DDSR_MASK   (0x01 << NS16550A_MSR_DDSR_OFFSET)
#define NS16550A_MSR_TERI_OFFSET 2  // Trailing edge ring indicator
#define NS16550A_MSR_TERI_MASK   (0x01 << NS16550A_MSR_TERI_OFFSET)
#define NS16550A_MSR_DCD_OFFSET  3  // Delta carrier detect
#define NS16550A_MSR_DCD_MASK    (0x01 << NS16550A_MSR_DCD_OFFSET)
#define NS16550A_MSR_CTS_OFFSET  4  // Clear to send
#define NS16550A_MSR_CTS_MASK    (0x01 << NS16550A_MSR_CTS_OFFSET)
#define NS16550A_MSR_DSR_OFFSET  5  // Data set ready
#define NS16550A_MSR_DSR_MASK    (0x01 << NS16550A_MSR_DSR_OFFSET)
#define NS16550A_MSR_RI_OFFSET   6  // Ring indicator
#define NS16550A_MSR_RI_MASK     (0x01 << NS16550A_MSR_RI_OFFSET)
#define NS16550A_MSR_CD_OFFSET   7  // Carrier detect
#define NS16550A_MSR_CD_MASK     (0x01 << NS16550A_MSR_CD_OFFSET)

// Scratch pad register
#define NS16550A_SPR_INDEX      0x07
#define NS16550A_SPR_UDA_OFFSET 0  // User data
#define NS16550A_SPR_UDA_MASK   (0xFF << NS16550A_SPR_UDA_OFFSET)

// Divisor latch LSB
#define NS16550A_DLL_INDEX      0x00
#define NS16550A_DLL_BDL_OFFSET 0  // Baudrate divisor constant LSB
#define NS16550A_DLL_BDL_MASK   (0xFF << NS16550A_DLL_BDL_OFFSET)

// Divisor latch MSB
#define NS16550A_DLM_INDEX      0x00
#define NS16550A_DLM_BDM_OFFSET 0  // Baudrate divisor constant MSB
#define NS16550A_DLM_BDM_MASK   (0xFF << NS16550A_DLM_BDM_OFFSET)

// Prescaler division
#define NS16550A_PSD_INDEX      0x05
#define NS16550A_PSD_PDF_OFFSET 0  // Prescaler division factor
#define NS16550A_PSD_PDF_MASK   (0x0F << NS16550A_PSD_PDF_OFFSET)

// Field accessors
#define NS16550A_READ_FIELD(uart, reg, field) \
    ((ns16550a_read_register(uart, (NS16550A_ ## reg ## _INDEX)) & \
            (NS16550A_ ## reg ## _ ## field ## _MASK)) >> \
        (NS16550A_ ## reg ## _ ## field ## _OFFSET))
#define NS16550A_WRITE_FIELD(uart, reg, field, value) \
    (ns16550a_write_register(uart, (NS16550A_ ## reg ## _INDEX), \
        (NS16550A_READ_FIELD(uart, reg, field) & \
             ~(NS16550A_ ## reg ## _ ## field ## _MASK)) | \
        (((value) << (NS16550A_ ## reg ## _ ## field ## _OFFSET)) & \
            (NS16550A_ ## reg ## _ ## field ## _MASK))))

typedef struct Ns16550aUart
{
    volatile uint8_t* base;
    size_t size;
    size_t register_width;
} Ns16550aUart;

extern const Ns16550aUart ns16550a_uart0;

uint8_t ns16550a_read_register(const Ns16550aUart* uart, uint8_t index);
void ns16550a_write_register(const Ns16550aUart* uart, uint8_t index,
    uint8_t value);

bool ns16550a_try_receive(const Ns16550aUart* uart, uint8_t* chr);
size_t ns16550a_receive_buffer(const Ns16550aUart* uart, uint8_t* data,
    size_t size);

bool ns16550a_transmit(const Ns16550aUart* uart, uint8_t chr);
size_t ns16550a_transmit_buffer(const Ns16550aUart* uart, const uint8_t* data,
    size_t size);

void ns16550a_initialize(void);
void ns16550a_finalize(void);

#endif  // KERNEL_DEVICE_NS16550A_NS16550A_H
