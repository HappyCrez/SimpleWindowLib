#pragma once
#include "pch.h"

namespace sw {

	class simple_window_api Window32API {
	public:
		Window32API() : Window32API(size, title) { }
		Window32API(Vector2u& size, std::string* title) : Window32API (location, size, title) { }
		Window32API(Vector2u& location, Vector2u& size, std::string* title);

		// WNDCLASS
		void createAndRegisterWindowClass();
		WNDCLASS createWindowClass();
		
		Vector2u getWindowSize(HWND hwnd);

		bool isOpen();

	private:
		Vector2u location = Vector2u(0, 0);
		Vector2u size = Vector2u(400, 300);
		HWND window_handle = nullptr;
		std::string* title = new std::string("Hello world");

		// Window
		HWND createWin(Vector2u& location, Vector2u& size, std::string* title);
		static LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		void processEvent(UINT message, WPARAM wparam, LPARAM lparam);
		bool pollEvent(Event& event);
		void processEvents();

		// Event queue
		std::queue<Event> event_queue;
		void pushEvent(Event& event);
	};

}