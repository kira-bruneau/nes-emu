CFLAGS += -g -I./src

EXTERNAL_LIBS = glfw3 gl glib-2.0 gio-2.0 portaudio-2.0
CFLAGS += $(shell pkg-config --cflags $(EXTERNAL_LIBS))
LDFLAGS += $(shell pkg-config --libs $(EXTERNAL_LIBS))

.PHONY: all
all: main loader

.PHONY: main
main: bin/main

bin/main: src/main.c src/ui/audio.c src/ui/render.c src/ui/events.c src/clock.c src/apu/apu.c
	mkdir -p bin/
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: loader
loader: bin/loader

bin/loader: src/loader.c src/nes.c src/cpu/cpu.c src/memory/memory.c src/cartridge/cartridge.c
	mkdir -p bin/
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf bin/
