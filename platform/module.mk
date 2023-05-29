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

platform/$(PLATFORM).KERNEL_CONFIG :=
$(eval $(call generate-module,platform/$(PLATFORM)))

# Validate the platform memory map and copy it to the kernel configuration.
define add-memory-map-entry-to-kernel-config
    ifeq ($(platform/$(PLATFORM).MEMORY_MAP.$(1)_BASE),)
        $$(error platform/$(PLATFORM) memory map does not specify a base for \
            entry $(1))
    endif

    platform/$(PLATFORM).KERNEL_CONFIG += \
        $(1)_BASE=$(platform/$(PLATFORM).MEMORY_MAP.$(1)_BASE)

    ifneq ($(platform/$(PLATFORM).MEMORY_MAP.$(1)_SIZE),)
        platform/$(PLATFORM).KERNEL_CONFIG += \
            $(1)_SIZE=$(platform/$(PLATFORM).MEMORY_MAP.$(1)_SIZE)
    endif
endef
$(foreach entry,$(platform/$(PLATFORM).MEMORY_MAP.ENTRIES), \
    $(eval $(call add-memory-map-entry-to-kernel-config,$(entry))))
ifneq ($(filter DRAM,$(platform/$(PLATFORM).MEMORY_MAP.ENTRIES)),DRAM)
    $(error platform/$(PLATFORM) memory map does not specify DRAM)
endif

MODULES += arch
