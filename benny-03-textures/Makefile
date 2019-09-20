CC = g++

LIBS += $(shell pkg-config --libs sdl2)
LIBS += $(shell pkg-config --libs glew)

CFLAGS += -I ./include
CFLAGS += $(shell pkg-config --cflags sdl2)
CFLAGS += $(shell pkg-config --cflags glew)

all: bin/app

bin/app: obj/main.o obj/display.o obj/shader.o obj/mesh.o obj/stb_image.o obj/texture.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

obj/main.o: src/main.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

obj/display.o: src/display.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

obj/shader.o: src/shader.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

obj/mesh.o: src/mesh.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

obj/stb_image.o: src/stb_image.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

obj/texture.o: src/texture.cpp
	$(CC) $(CFLAGS) -o $@ -c $^
