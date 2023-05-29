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

# Upon inclusion, this file will generate all modules in $(MODULES) that have
# not yet been generated, then recursively include itself to generate any
# modules required by the modules included in the previous step.

MODULES := $(filter-out $(MODULES_GENERATED),$(sort $(MODULES)))
ifneq ($(MODULES),)
    MODULES_TO_GENERATE := $(MODULES)
    MODULES :=
    $(foreach module,$(MODULES_TO_GENERATE), \
        $(eval $(call generate-module,$(module))))
    $(eval $(call generate-modules))
endif
