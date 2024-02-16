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

clean:
	rm -r build

install:
	cp pixelball /usr/bin/pixelball
	chmod a+x /usr/bin/pixelball
	xdg-icon-resource install --novendor --size 16 assets/icon-16.xpm pixelball
	xdg-icon-resource install --novendor --size 32 assets/icon-32.xpm pixelball
	xdg-icon-resource install --novendor --size 64 assets/icon-64.xpm pixelball
	xdg-desktop-menu install --novendor pixelball.desktop

uninstall:
	rm /usr/bin/pixelball
	xdg-icon-resource uninstall --size 16 pixelball
	xdg-icon-resource uninstall --size 32 pixelball
	xdg-icon-resource uninstall --size 64 pixelball
	xdg-desktop-menu uninstall pixelball.desktop

