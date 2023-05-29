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

# Get the directory of the current makefile.
get-makefile-dir = $(patsubst ./,,$(dir $(lastword $(MAKEFILE_LIST))))

# Get the tail of a list $(1).
tail = $(strip $(wordlist 2,$(words $(1)),x $(1)))

# Canned recipe: Run a recipe command $(2) and echo the full command if
# VERBOSE is set or the summary $(1) if VERBOSE is not set.
VERBOSE := $(filter 1,$(VERBOSE))
define run-command
    $(if $(VERBOSE),,@echo "$(1)")
    $(if $(VERBOSE),,@)$(2)
endef
