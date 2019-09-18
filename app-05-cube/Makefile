
LIBS += -ldl
LIBS += -lglfw
LIBS += -lm

CFLAGS += -I ./include

all: app

app: glad.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
