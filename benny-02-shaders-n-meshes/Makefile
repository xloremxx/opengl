CC = g++

LIBS += $(shell pkg-config --libs sdl2)
LIBS += $(shell pkg-config --libs glew)

CFLAGS += -I ./include
CFLAGS += $(shell pkg-config --cflags sdl2)
CFLAGS += $(shell pkg-config --cflags glew)

all: app

app: main.o display.o shader.o mesh.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

main.o: main.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

display.o: display.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

shader.o: shader.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

mesh.o: mesh.cpp
	$(CC) $(CFLAGS) -o $@ -c $^
