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

private:
	Point location;
	std::string title;
	bool visible;
public:
	// Constructors
	Window(Point location, std::string title) {
		Window::location = location;
		Window::title = title;
	}
	Window() {
		Window::location = Point(0, 0);
		Window::title = "new Window";
	}

	// Functions
	void setTitle(std::string winTitle);
	void set_visible(bool visible);
};