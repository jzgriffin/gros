# Copyright (c) 2023 Jeremiah Z. Griffin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

$(MODULE).CONFIG_CMD := \
    $(PYTHON) \
    $(call get-makefile-dir)config.py \
    --subarch $(SUBARCH) \
    --abi $(ABI) \
    --mmu $(MMU)

$(MODULE).KERNEL_CONFIG := $(shell $($(MODULE).CONFIG_CMD) kernel-config)
ifneq ($(.SHELLSTATUS),0)
    $(error Unable to get kernel configuration from `$($(MODULE).CONFIG_CMD) \
        kernel-config`)
endif

ARCH_RISCV_XLEN := $(shell $($(MODULE).CONFIG_CMD) xlen)
ARCH_RISCV_CFLAGS := $(shell $($(MODULE).CONFIG_CMD) cflags)

CROSS_COMPILE ?= riscv$(ARCH_RISCV_XLEN)-elf-
CFLAGS += -mcmodel=medany $(ARCH_RISCV_CFLAGS)
