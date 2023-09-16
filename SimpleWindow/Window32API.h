#pragma once
#include "pch.h"

namespace sw {

	class simple_window_api Window32API {
	public:

		Window32API();

		// WNDCLASS
		void createAndRegisterWindowClass();
		WNDCLASS createWindowClass();

		// Window
		HWND createWin(Vector2u& location, Vector2u& size, std::string title);
		static LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		
		Vector2u getWindowSize(HWND hwnd);
		void processEvent(UINT message, WPARAM wparam, LPARAM lparam);
		bool pollEvent(Event& event);
		void processEvents();

		bool isOpen();

		Vector2u location = Vector2u(0, 0);
		Vector2u size = Vector2u(400, 300);
		HWND window_handle = nullptr;
		std::string title = "Hello world";

		// Event queue
		std::queue<Event> event_queue;
		void pushEvent(Event& event);
	private:
	};

}