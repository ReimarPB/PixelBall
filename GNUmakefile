OBJS=build/main.o build/native/x11.o build/native/common.o build/entities/ball.o build/entities/block.o build/entities/particle.o build/scenes/game.o build/scenes/pause.o build/ui/text.o
LIBS=-lX11 -lXpm -lXext -lpthread

_ := $(shell mkdir -p build/native build/entities build/scenes build/ui)

build/%.o: %.c
	$(CC) -c -o $@ $< $(LIBS)

all: $(OBJS)
	$(CC) -o pixelball $(OBJS) $(LIBS)

debug: $(OBJS)
	$(CC) -o pixelball -g -fsanitize=leak $(OBJS) $(LIBS)

clean:
	rm -r build

