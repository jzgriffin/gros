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

# Generates a submodule by its directory $(1).  MODULE must be set to the
# parent module.
define generate-submodule
    ifeq ($(filter $(1),$(SUBMODULES_GENERATED)),)
        SUBMODULES_GENERATED := $(sort $(SUBMODULES_GENERATED) $(1))
        SUBMODULE_STACK := $$(strip $$(SUBMODULE_STACK) $(SUBMODULE))
        SUBMODULE := $(MODULE)/$(1)
        $$(SUBMODULE).INNER_SRC_DIR := $(1)/
        $$(SUBMODULE).SRC_DIR := $(MODULE)/$(1)/
        $$(info Generating $$(SUBMODULE))
        include $$($$(SUBMODULE).SRC_DIR)submodule.mk
        $$(foreach type,$$($$(MODULE).TYPES), \
            $$(eval -include make/generator/$$(type)-submodule.mk))
        SUBMODULE := $$(lastword $$(SUBMODULE_STACK))
        SUBMODULE_STACK := $$(call tail,$$(SUBMODULE_STACK))
    endif
endef

# Generates the submodules in $(SUBMODULES).
define generate-submodules
    include make/generate-submodules.mk
endef

# Generates a module by its directory $(1).
define generate-module
    ifeq ($(filter $(1),$(MODULES_GENERATED)),)
        MODULES_GENERATED := $(sort $(MODULES_GENERATED) $(1))
        MODULE_STACK := $$(strip $$(MODULE_STACK) $(MODULE))
        MODULE := $(1)
        $$(MODULE).SRC_DIR := $(1)/
        $$(MODULE).OUT_DIR := $(OUT_DIR)$$($$(MODULE).SRC_DIR)
        $$(info Generating $$(MODULE))
        include $$($$(MODULE).SRC_DIR)module.mk
        $$(eval $$(call generate-submodules))
        $$(foreach type,$$($$(MODULE).TYPES), \
            $$(eval include make/generator/$$(type)-module.mk))
        MODULE := $$(lastword $$(MODULE_STACK))
        MODULE_STACK := $$(call tail,$$(MODULE_STACK))
    endif
endef

# Generates the modules in $(MODULES).
define generate-modules
    include make/generate-modules.mk
endef
