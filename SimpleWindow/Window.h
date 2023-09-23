#pragma once
#include "pch.h"
#include "Event.h"
#include "Widget.h"

namespace sw {

	class simple_window_api Window {
	public:
		Window() : Window(Vector2u(1200, 800), "Hello world") { }
		Window(Vector2u size, std::string title) : Window (Vector2i(0, 0), size, title) { }
		Window(Vector2i position, Vector2u size, std::string title);
		
		bool isOpen();
		void close();

		bool pollEvent(Event& event);
		void add(Widget& widget);

		void setSize(Vector2u& size);
		void setPosition(Vector2i& position);
		void setParams(Vector2i& position, Vector2u& size);

		Vector2u getSize();
		HWND getHandle();

	private:
		Vector2i position;
		Vector2u size;
		HWND handle = nullptr;
		std::string title;

		// WNDCLASS
		void createAndRegisterWindowClass();
		WNDCLASS createWindowClass();

		// Window
		HWND createWin(Vector2i position, Vector2u size, std::string title);
		static LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		void processEvent(UINT message, WPARAM wparam, LPARAM lparam);
		void processEvents();

		// Event queue
		std::queue<Event> events;
		void pushEvent(Event& event);

		// Widget
		long int getWidgetFlagsByType(Widget& widget);
	};
}