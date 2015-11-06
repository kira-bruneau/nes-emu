CC = gcc
CFLAGS = -g -I./src

MAIN_SRCS = main clock ui/render ui/events ui/audio apu/apu
LOADER_SRCS = loader nes cpu/cpu memory/memory cartridge/cartridge
PKGCONFIG = glfw3 gl glib-2.0 gio-2.0 portaudio-2.0

BINDIR = bin
OBJDIR = obj
SRCDIR = src

CFLAGS += $(shell pkg-config --cflags $(PKGCONFIG))
LDFLAGS += $(shell pkg-config --libs $(PKGCONFIG))

.PHONY: all
all: main loader

# Main code #
.PHONY: main
main: bin/main
$(BINDIR)/main: $(addprefix $(OBJDIR)/, $(addsuffix .o, $(MAIN_SRCS)))
	@mkdir -p $(shell dirname $@)
	$(CC) $(LDFLAGS) -o $@ $^

-include $(addprefix $(OBJDIR)/, $(addsuffix .d, $(MAIN_SRCS)))

# Code for loading iNES ROMs and testing the CPU (will be merged into main) #
.PHONY: loader
loader: bin/loader
$(BINDIR)/loader: $(addprefix $(OBJDIR)/, $(addsuffix .o, $(LOADER_SRCS)))
	@mkdir -p $(shell dirname $@)
	$(CC) $(LDFLAGS) -o $@ $^

-include $(addprefix $(OBJDIR)/, $(addsuffix .d, $(LOADER_SRCS)))

# Generic rule to build object files #
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)
