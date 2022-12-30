#include "common.h"

enum event_type {
	DRAW;
	KEY_DOWN;
	KEY_UP;
};

struct draw_event_data {
	int x;
	int y;
	int width;
	int height;
};

union event_data {
	struct draw_event_data area;
	enum key key;
}

typedef void (*event_callback)(union event_data data);

struct event_listener {
	enum event_type type;
	event_callback callback;
};

void dispatch_event(enum event_type type, union event_data data);

void add_event_listener(enum event_type type, event_callback callback);

