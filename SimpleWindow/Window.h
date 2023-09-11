#pragma once
#include "pch.h"
#include <string>

#ifndef SIMPLEWINDOW_EXPORTS
#define simple_window_api __declspec(dllexport)
#else
#define simple_window_api __declspec(dllimport)
#endif

extern "C" simple_window_api
struct Window {
	Point *location;
	std::string title;
	bool visible;

	// Constructors
	Window(Point* location, std::string title, bool visible) {
		Window::location = location;
		Window::title = title;
		Window::visible = visible;
	}
	Window() {
		Window::location = new Point(0, 0);
		Window::title = "new Window";
		Window::visible = false;
	}
};

