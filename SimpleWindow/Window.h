#pragma once
#include "pch.h"
#include "Event.h"
#include "Widget.h"

namespace sw {


	struct simple_window_api Window {
		int events_count = 0;
		HWND handle = nullptr;
		Event events[50] = { };
		
		Vector2i position;
		Vector2u size;
		char* title;
	};

	extern "C" simple_window_api
	bool windowIsOpen(Window& window);
	extern "C" simple_window_api
	void windowClose(Window& window);

	extern "C" simple_window_api
	void windowSetSize(Window& window, Vector2u& size);
	extern "C" simple_window_api
	void windowSetPosition(Window& window, Vector2i& position);
	extern "C" simple_window_api
	void windowSetPositionAndSize(Window& window, Vector2i& position, Vector2u& size);

	extern "C" simple_window_api
	void initWindowZeroParams(Window& window);
	extern "C" simple_window_api
	void initWindowTwoParams(Window& window, Vector2u size, char* title);
	extern "C" simple_window_api
	void initWindow(Window& window, Vector2i position, Vector2u size, char* title);

	extern "C" simple_window_api
	void createAndRegisterWindowClass(Window& window);
	extern "C" simple_window_api
	WNDCLASS createWindowClass(Window& window);

	extern "C" simple_window_api
	HWND windowCreate(Window* window, Vector2i position, Vector2u size, char* title);
	extern "C" simple_window_api
	LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	extern "C" simple_window_api
	void windowPushEvent(Window* window, Event& event);
	extern "C" simple_window_api
	bool windowPollEvent(Window& window, Event& event);
	extern "C" simple_window_api
	void windowProcessEvents(Window& window);
	extern "C" simple_window_api
	void windowProcessEvent(Window* window, UINT message, WPARAM wparam, LPARAM lparam);

	extern "C" simple_window_api
	void windowAddWidget(Window& window, Widget& widget);
	extern "C" simple_window_api
	long int getWidgetFlagsByType(WidgetType widgetType, Font widgetFont);
}