#pragma once
#include "pch.h"

namespace sw {

	class simple_window_api Window {
	public:
		Window() : Window(Vector2u(400, 300), "Hello world") { }
		Window(Vector2u size, std::string title) : Window (Vector2u(0, 0), size, title) { }
		Window(Vector2u location, Vector2u size, std::string title);
		
		Vector2u getWindowSize(HWND hwnd);

		bool isOpen();
		void close();
		bool pollEvent(Event& event);

		void setSize(Vector2u& size);
		void setLocation(Vector2u& location);
		void setParams(Vector2u& location, Vector2u& size);

		Vector2u getSize();
		HWND getHandle();

	private:
		Vector2u location;
		Vector2u size;
		HWND window_handle = nullptr;
		std::string title;

		// WNDCLASS
		void createAndRegisterWindowClass();
		WNDCLASS createWindowClass();

		// Window
		HWND createWin(Vector2u location, Vector2u size, std::string title);
		static LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		void processEvent(UINT message, WPARAM wparam, LPARAM lparam);
		void processEvents();

		// Event queue
		std::queue<Event> event_queue;
		void pushEvent(Event& event);
	};

}