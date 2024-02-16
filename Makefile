OBJS=                             \
	build/main.o                  \
	build/native/x11.o            \
	build/native/common.o         \
	build/entities/ball.o         \
	build/entities/block.o        \
	build/entities/particle.o     \
	build/scenes/scene.o          \
	build/scenes/game.o           \
	build/scenes/pause.o          \
	build/scenes/main_menu.o      \
	build/scenes/transition.o     \
	build/ui/text.o               \
	build/ui/button.o             \
	build/utils/position.o        \
	build/components/background.o \
	build/levels/level.o          \
	build/levels/test_level.o
LIBS=-lX11 -lXpm -lXext -lXrender -lpthread -lstartup-notification-1

build/%.o: %.c $(wildcard *.h) $(wildcard *.xpm)
	mkdir -p `dirname $@`
	$(CC) -c -o $@ $< $(LIBS)

build/%.o: %.pblv
	mkdir -p `dirname $@`
	objcopy --input binary --output elf64-x86-64 $< $@

all: $(OBJS)
	$(CC) -o pixelball $(OBJS) $(LIBS)

debug: $(OBJS)
	$(CC) -o pixelball -g -pg -fsanitize=leak $(OBJS) $(LIBS)

install:
	cp pixelball /usr/bin/pixelball

clean:
	rm -r build

