OBJS=build/main.o build/native/x11.o build/entities/ball.o build/entities/block.o build/entities/particle.o build/scenes/game.o
LIBS=-lX11 -lXpm -lXext -lpthread

_ := $(shell mkdir -p build/native build/entities build/scenes)

build/%.o: %.c
	$(CC) -c -o $@ $< $(LIBS)

all: $(OBJS)
	$(CC) -o pixelball $(OBJS) $(LIBS)

clean:
	rm -r build

