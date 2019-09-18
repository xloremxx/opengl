
LIBS += -ldl
LIBS += -lglfw
LIBS += -lm

CFLAGS += -I ./include

all: app

app: glad.o main.o stb_image.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
