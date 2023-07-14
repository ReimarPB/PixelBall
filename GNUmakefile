OBJS=build/main.o build/native/x11.o build/native/common.o build/entities/ball.o build/entities/block.o build/entities/particle.o build/scenes/scene.o build/scenes/game.o build/scenes/pause.o build/scenes/main_menu.o build/scenes/transition.o build/ui/text.o build/ui/button.o build/utils/position.o build/components/background.o
LIBS=-lX11 -lXpm -lXext -lXrender -lpthread

_ := $(shell mkdir -p build/native build/entities build/scenes build/ui build/utils build/components)

build/%.o: %.c
	$(CC) -c -o $@ $< $(LIBS)

all: $(OBJS)
	$(CC) -o pixelball $(OBJS) $(LIBS)

debug: $(OBJS)
	$(CC) -o pixelball -g -fsanitize=leak $(OBJS) $(LIBS)

clean:
	rm -r build

