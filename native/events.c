#include "events.h"

struct event_listener *listeners;
int listener_count = 0;

void dispatch_event(enum event_type type, union event_data data)
{
	for (int i = 0; i < listener_count; i++) {
		if (listeners[i].type == type)
			listeners[i].callback(data);
	}
}

void add_event_listener(enum event_type type, event_callback callback)
{
	listeners = realloc((listener_count + 1) * sizeof(struct event_listener));
	listeners[listener_count++] = (struct event_listener) {
		.type = type,
		.callback = callback
	};
}

