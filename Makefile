EXTERNAL_LIBS=glfw3 gl

CFLAGS += `pkg-config --cflags $(EXTERNAL_LIBS)`
LDFLAGS += `pkg-config --libs $(EXTERNAL_LIBS)`

main: main.c render.c events.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -rf main
