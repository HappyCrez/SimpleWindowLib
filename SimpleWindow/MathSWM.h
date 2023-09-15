#pragma once
#include "pch.h"

namespace sw {

	struct simple_window_api Vector2i {
		int x;
		int y;
	
		Vector2i() : Vector2i(0, 0) { }

		Vector2i(int x, int y) {
			Vector2i::x = x;
			Vector2i::y = y;
		}
	};

	struct simple_window_api Vector2u {
		unsigned int x, y;

		Vector2u() : Vector2u(0, 0) { }

		Vector2u(unsigned int x, unsigned int y) {
			Vector2u::x = x;
			Vector2u::y = y;
		}
	};

	struct simple_window_api Vector2f {
		float x, y;

		Vector2f() : Vector2f(0, 0) { }

		Vector2f(float x, float y) {
			Vector2f::x = x;
			Vector2f::y = y;
		}
	};
}