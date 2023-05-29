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

# Disable the builtin makefile variables and rules to improve performance and
# prevent unexpected or difficult-to-trace behavior.
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --no-builtin-rules

# Include the user configuration.
-include config.mk

# Set the default output directory.
OUT_DIR ?= out/

# Set the default product.
PRODUCT ?= core

# Include the makefile library.
include make/command.mk
include make/flag.mk
include make/macro.mk
include make/module.mk

# Create the top-level meta targets.
.PHONY: all clean distclean
all::
clean::
distclean::
	$(RM) -r $(OUT_DIR)

# Generate the product module, which generates all other modules.
MODULES := product
$(eval $(call generate-modules))
