#pragma once
#include "pch.h"

namespace sw {

	struct simple_window_api Vector2i {
		int x;
		int y;
	
		Vector2i(int x, int y) {
			Vector2i::x = x;
			Vector2i::y = y;
		}

		Vector2i() {
			x = 0;
			y = 0;
		}
	};

	struct simple_window_api Vector2u {
		unsigned int x, y;

		Vector2u(unsigned int x, unsigned int y) {
			Vector2u::x = x;
			Vector2u::y = y;
		}

		Vector2u() {
			x = 0;
			y = 0;
		}
	};
}