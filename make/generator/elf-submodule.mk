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

# Make paths relative to the top directory.
$(SUBMODULE).SRCS := \
    $(addprefix $($(SUBMODULE).INNER_SRC_DIR),$($(SUBMODULE).SRCS))
$(SUBMODULE).INC_DIRS := \
    $(addprefix $($(SUBMODULE).INNER_SRC_DIR),$($(SUBMODULE).INC_DIRS))
$(SUBMODULE).LDS := \
    $(addprefix $($(SUBMODULE).INNER_SRC_DIR),$($(SUBMODULE).LDS))

# Append the submodule variables to the module.
$(MODULE).CFLAGS := $(strip $($(MODULE).CFLAGS) $($(SUBMODULE).CFLAGS))
$(MODULE).ASFLAGS := $(strip $($(MODULE).ASFLAGS) $($(SUBMODULE).ASFLAGS))
$(MODULE).LDFLAGS := $(strip $($(MODULE).LDFLAGS) $($(SUBMODULE).LDFLAGS))
$(MODULE).SRCS := $(strip $($(MODULE).SRCS) $($(SUBMODULE).SRCS))
$(MODULE).LDS := $(strip $($(MODULE).LDS) $($(SUBMODULE).LDS))
$(MODULE).INC_DIRS := \
    $(strip $($(MODULE).INC_DIRS) $($(SUBMODULE).INC_DIRS))
