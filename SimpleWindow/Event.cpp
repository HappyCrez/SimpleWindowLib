#pragma once
#include "pch.h"
#include "Event.h"

namespace sw {

	
	bool pollEvent(Event& event) {
		if (event_queue.empty()) return false;
		event = event_queue.front();
		event_queue.pop();
		return true;
	}

	void pushEvent(Event& event) {
		event_queue.push(event);
	}

	Event popEvent() {
		if (event_queue.empty()) return Event();
		Event event = event_queue.front();
		event_queue.pop();
		return event;
	}
}