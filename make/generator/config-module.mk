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

$(MODULE).CONFIG.OUT_DIR := $($(MODULE).OUT_DIR)_config/include/
$(MODULE).CONFIG.OBJ_DIR := $($(MODULE).OUT_DIR)_config/object/

# Make paths relative to the top directory.
$(MODULE).CONFIG.OBJ_LST := \
    $(patsubst %.in,$($(MODULE).CONFIG.OBJ_DIR)%.o,$($(MODULE).CONFIG.SRC))
$(MODULE).CONFIG.OBJ := \
    $(patsubst %.in,$($(MODULE).CONFIG.OBJ_DIR)%,$($(MODULE).CONFIG.SRC))
$(MODULE).CONFIG.SRC := \
    $(addprefix $($(MODULE).SRC_DIR),$($(MODULE).CONFIG.SRC))
$(MODULE).CONFIG.OUT := \
    $(addprefix $($(MODULE).CONFIG.OUT_DIR),$($(MODULE).CONFIG.OUT))

# Unconditionally write the object file.
$(MODULE).CONFIG := $(sort $($(MODULE).CONFIG))
$(MODULE).CONFIG.LST := $(shell $(RM) $($(MODULE).CONFIG.OBJ_LST))
$(MODULE).CONFIG.LST := $(shell $(MKDIR) $(dir $($(MODULE).CONFIG.OBJ_LST)))
$(foreach item,$($(MODULE).CONFIG), \
    $(eval $$(shell echo "$(item)" >> $($(MODULE).CONFIG.OBJ_LST))))
$(MODULE).CONFIG.LST := \
    $(shell $(PYTHON) make/generator/config.py \
	generate \
        $($(MODULE).CONFIG.SRC) \
	$($(MODULE).CONFIG.OBJ_LST) \
	$($(MODULE).CONFIG.OBJ))

# Generate the module output.
$(MODULE).EXTRA_PREREQS := $(strip \
    $($(MODULE).EXTRA_PREREQS) \
    $($(MODULE).CONFIG.OUT) \
    )
$($(MODULE).CONFIG.OUT): MODULE := $(MODULE)
$($(MODULE).CONFIG.OUT): $($(MODULE).CONFIG.SRC) $($(MODULE).CONFIG.OBJ)
	@$(PYTHON) make/generator/config.py \
	    deploy \
	    $($(MODULE).CONFIG.OBJ) \
	    $($(MODULE).CONFIG.OUT)

# Add the output directory the module include directories.
$(MODULE)._INC_DIRS := $(strip \
    $($(MODULE)._INC_DIRS) \
    $($(MODULE).CONFIG.OUT_DIR) \
    )
