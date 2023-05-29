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

$(MODULE).OUT_EXT ?= .elf
$(MODULE).OUT := $($(MODULE).OUT_DIR)$(MODULE)$($(MODULE).OUT_EXT)

# Make paths relative to the top directory.
$(MODULE).SRCS := $(addprefix $($(MODULE).SRC_DIR),$($(MODULE).SRCS))
$(MODULE).LDS := $(addprefix $($(MODULE).SRC_DIR),$($(MODULE).LDS))
$(MODULE).INC_DIRS := $(strip \
    $(addprefix $($(MODULE).SRC_DIR),$($(MODULE).INC_DIRS)) \
    $($(MODULE)._INC_DIRS) \
    )

# Generate the module object toolchain flags.
$(MODULE)._CFLAGS := $(strip \
    $(CFLAGS) \
    $($(MODULE).CFLAGS) \
    $(addprefix -I,$($(MODULE).INC_DIRS)) \
    )
$(MODULE)._ASFLAGS := $(strip \
    $(ASFLAGS) \
    $($(MODULE).ASFLAGS) \
    )

# Generate the module objects.
$(MODULE).PREREQS := $($(MODULE).PREREQS)
$(MODULE).OBJS := $($(MODULE).OBJS)
$(MODULE).DEPS := $($(MODULE).DEPS)

# .lds.S files
$(MODULE).SRCS.lds := $(filter %.lds.S,$($(MODULE).LDS))
$(MODULE).OBJS.lds := \
  $(patsubst %.lds.S,$(OUT_DIR)%.lds,$($(MODULE).SRCS.lds))
$(MODULE).DEPS.lds := $(patsubst %.lds,%.lds.d,$($(MODULE).OBJS.lds))
$(MODULE).PREREQS += $($(MODULE).OBJS.lds)
$(MODULE).DEPS += $($(MODULE).DEPS.lds)
$($(MODULE).OBJS.lds): MODULE := $(MODULE)
$($(MODULE).OBJS.lds): $($(MODULE).EXTRA_PREREQS)
$($(MODULE).OBJS.lds): $(OUT_DIR)%.lds: %.lds.S
	@$(MKDIR) $(dir $@)
	$(call run-command,CPP $<, \
	    $(CPP) \
	        $($(MODULE)._CFLAGS) \
		-MMD -MP -MT $@ -MF $(@:%.lds=%.lds.d) \
		-o $@ $<)

# .c files
$(MODULE).SRCS.c := $(filter %.c,$($(MODULE).SRCS))
$(MODULE).OBJS.c := $(patsubst %.c,$(OUT_DIR)%.c.o,$($(MODULE).SRCS.c))
$(MODULE).DEPS.c := $(patsubst %.o,%.d,$($(MODULE).OBJS.c))
$(MODULE).OBJS += $($(MODULE).OBJS.c)
$(MODULE).DEPS += $($(MODULE).DEPS.c)
$($(MODULE).OBJS.c): MODULE := $(MODULE)
$($(MODULE).OBJS.c): $($(MODULE).EXTRA_PREREQS)
$($(MODULE).OBJS.c): $(OUT_DIR)%.c.o: %.c
	@$(MKDIR) $(dir $@)
	$(call run-command,CC $<, \
	    $(CC) \
	        $($(MODULE)._CFLAGS) \
		-MMD -MP -MT $@ -MF $(@:%.o=%.d) \
		-o $@ -c $<)

# .S files
$(MODULE).SRCS.S := $(filter %.S,$($(MODULE).SRCS))
$(MODULE).OBJS.S := $(patsubst %.S,$(OUT_DIR)%.S.o,$($(MODULE).SRCS.S))
$(MODULE).DEPS.S := $(patsubst %.o,%.d,$($(MODULE).OBJS.S))
$(MODULE).OBJS += $($(MODULE).OBJS.S)
$(MODULE).DEPS += $($(MODULE).DEPS.S)
$($(MODULE).OBJS.S): MODULE := $(MODULE)
$($(MODULE).OBJS.S): $($(MODULE).EXTRA_PREREQS)
$($(MODULE).OBJS.S): $(OUT_DIR)%.S.o: %.S
	@$(MKDIR) $(dir $@)
	$(call run-command,CC $<, \
	    $(CC) \
	        $($(MODULE)._CFLAGS) \
	        $($(MODULE)._ASFLAGS) \
		-MMD -MP -MT $@ -MF $(@:%.o=%.d) \
		-o $@ -c $<)

# Configure the module output toolchain flags.
$(MODULE)._LDFLAGS := $(strip \
    $(LDFLAGS) \
    $($(MODULE).LDFLAGS) \
    $(foreach lds,$($(MODULE).OBJS.lds),-Wl,-T$(lds)) \
    )

# Generate the module output.
$($(MODULE).OUT): MODULE := $(MODULE)
$($(MODULE).OUT): $($(MODULE).EXTRA_PREREQS)
$($(MODULE).OUT): $($(MODULE).PREREQS)
$($(MODULE).OUT): $($(MODULE).OBJS)
	@$(MKDIR) $(dir $@)
	$(call run-command,LD $@, \
            $(CC) \
	        $($(MODULE)._CFLAGS) \
	        $($(MODULE)._LDFLAGS) \
		-o $@ $($(MODULE).OBJS))

# Create the module meta-rules for compiled targets.
.PHONY: $(MODULE) clean-$(MODULE) distclean-$(MODULE)
$(MODULE): $($(MODULE).OUT)
clean-$(MODULE): MODULE := $(MODULE)
clean-$(MODULE):
	$(RM) $($(MODULE).OBJS) $($(MODULE).DEPS)
distclean-$(MODULE): MODULE := $(MODULE)
distclean-$(MODULE):
	$(RM) -r $($(MODULE).OUT_DIR)

# Add the module to the default meta-rules.
all:: $(MODULE)
clean:: clean-$(MODULE)

# Include the dependency files.
-include $($(MODULE).DEPS)
