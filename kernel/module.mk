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

# Ensure that configuration is complete before generating the kernel.  The
# kernel module depends on variables such as BOARD, PLATFORM, ARCH, etc. which
# are populated by the product configuration.
$(eval $(call generate-module,product))

$(MODULE).TYPES := config elf

$(MODULE).SRCS := \
    main.c
$(MODULE).INC_DIRS := include

$(MODULE).CONFIG.SRC := include/kernel/config.h.in
$(MODULE).CONFIG.OUT := kernel/config.h
$(MODULE).CONFIG := $(strip \
    $(product/$(PRODUCT).KERNEL_CONFIG) \
    $(board/$(BOARD).KERNEL_CONFIG) \
    $(platform/$(PLATFORM).KERNEL_CONFIG) \
    $(arch/$(ARCH).KERNEL_CONFIG) \
    )

SUBMODULES := \
    $(board/$(BOARD).KERNEL_DEVICES) \
    $(platform/$(PLATFORM).KERNEL_DEVICES) \
    arch/$(ARCH) \
    lib
