LIBS += -ldl
LIBS += -lglfw
CFLAGS += -I ./include

all: app

app: glad.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
