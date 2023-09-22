#pragma once
#include "pch.h"
#include "Event.h"
#include "Widget.h"

namespace sw {

	struct simple_window_api Window {
		Vector2u position;
		Vector2u size;
		HWND handle = nullptr;
		std::string title;
		std::queue<Event> events;
	};

	extern "C" simple_window_api
	bool isOpen(Window& window);
	extern "C" simple_window_api
	void close(Window& window);

	extern "C" simple_window_api
	void setSize(Window& window, Vector2u& size);
	extern "C" simple_window_api
	void setPosition(Window& window, Vector2u& position);
	extern "C" simple_window_api
	void setParams(Window& window, Vector2u& position, Vector2u& size);

	extern "C" simple_window_api
	void initWindowZeroParams(Window& window);
	extern "C" simple_window_api
	void initWindowTwoParams(Window& window, Vector2u size, std::string title);
	extern "C" simple_window_api
	void initWindowThreeParams(Window& window, Vector2u position, Vector2u size, std::string title);

	extern "C" simple_window_api
	void createAndRegisterWindowClass(Window& window);
	extern "C" simple_window_api
	WNDCLASS createWindowClass(Window& window);

	extern "C" simple_window_api
	HWND createWin(Window* window, Vector2u position, Vector2u size, std::string title);
	extern "C" simple_window_api
	LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	extern "C" simple_window_api
	void pushEvent(Window* window, Event& event);
	extern "C" simple_window_api
	bool pollEvent(Window& window, Event& event);
	extern "C" simple_window_api
	void processEvents(Window& window);
	extern "C" simple_window_api
	void processEvent(Window* window, UINT message, WPARAM wparam, LPARAM lparam);

	extern "C" simple_window_api
	void add(Window& window, Widget& widget);
	extern "C" simple_window_api
	long int getWidgetFlagsByType(Widget& widget);
}