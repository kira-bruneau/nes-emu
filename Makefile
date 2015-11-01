EXTERNAL_LIBS=glfw3 gl glib-2.0 gio-2.0 portaudio-2.0

CFLAGS += '-g'

CFLAGS += '-I./src'

CFLAGS += `pkg-config --cflags $(EXTERNAL_LIBS)`
LDFLAGS += `pkg-config --libs $(EXTERNAL_LIBS)`

.PHONY: all
all: main loader apu-test

.PHONY: main
main: bin/main

bin/main: src/main.c src/ui/render.c src/ui/events.c
	mkdir -p bin/
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: loader
loader: bin/loader

bin/loader: src/loader.c src/nes.c src/cpu/cpu.c src/memory/memory.c src/cartridge/cartridge.c
	mkdir -p bin/
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: apu-test
apu-test: bin/apu-test

bin/apu-test: src/apu-test.c src/ui/audio.c src/struct/buffer.c src/apu/apu.c src/apu/pulse.c src/apu/triangle.c src/apu/noise.c src/apu/dmc.c
	mkdir -p bin/
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf bin/
