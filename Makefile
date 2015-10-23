EXTERNAL_LIBS=glfw3 gl glib-2.0 gio-2.0

CFLAGS += `pkg-config --cflags $(EXTERNAL_LIBS)`
LDFLAGS += `pkg-config --libs $(EXTERNAL_LIBS)`

main: main.c render.c events.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

loader: loader.c nes.c cpu.c memory.c cartridge.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -rf main loader
