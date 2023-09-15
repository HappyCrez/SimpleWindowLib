#pragma once
#include "pch.h"

namespace sw {

	void createAndRegisterWindowClass();

	LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	WNDCLASS createWNDCLASS();

	HWND createWin(Vector2u& location, Vector2u& size, std::string title);

	class simple_window_api Window {
	private:
		HWND m_handle;
		Vector2u location;
		Vector2u size;
		std::string title;

	public:
		Window () : Window("New Window") {
			
		}

		Window(std::string title) :
			Window(Vector2u(0, 0),		// Location
			Vector2u(300, 300),			// Size
			title) {

		}

		Window (Vector2u location, Vector2u size, std::string title) {
			Window::location = location;
			Window::size = size;
			Window::title = title;
			m_handle = nullptr;

			std::thread processWindow(
				std::bind (& Window::initWindow, this,
				location, size, title)
				);
			processWindow.detach();

			while (!Window::m_handle) {
				// Wait window creation
				Sleep(1000);
			}
		}

		bool isOpen();

		void setSize(Vector2u& size);

		void setLocation(Vector2u& location);

		void setParams(Vector2u& location, Vector2u& size);

		Vector2u getSize();

	private:
		void initWindow(Vector2u& location, Vector2u& size, std::string title);
		void processMessages();
		void clear();
	};

	Vector2u getWindowSize(HWND hwnd);
}