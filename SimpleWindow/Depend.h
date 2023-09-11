#pragma once

struct Point {
	public:

	// Fields
	int x;
	int y;
	
	// Constuctors
	Point(int x, int y) {
		Point::x = x;
		Point::y = y;
	}
	Point() {
		x = 0;
		y = 0;
	}
};