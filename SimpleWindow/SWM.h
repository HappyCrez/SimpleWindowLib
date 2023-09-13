#pragma once
#include "pch.h"
#include <string>

#ifdef SIMPLEWINDOW_EXPORTS
#define simple_window_api __declspec(dllexport)
#else
#define simple_window_api __declspec(dllimport)
#endif


namespace sw {

	void createAndRegisterWindowClass();

	LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	
	extern "C++" simple_window_api
	WNDCLASS createWNDCLASS();

	extern "C++" simple_window_api
	HWND initializeWindow(Vector2u & location, Vector2u & size, std::string title);

	extern "C++" simple_window_api
	HWND initializeWindow();

	struct Window {
		HWND m_handle;
		Vector2u location = Vector2u(0, 0);
		Vector2u size = Vector2u(300, 300);

		Window() {
			m_handle = initializeWindow();
		}
	};
	
	extern "C++" simple_window_api
	bool isWindowOpen(Window & window);

	extern "C++" simple_window_api
	void pollEvent(Window & window);

	extern "C++" simple_window_api
	void setWindowSize(Window& window, Vector2u& size);

	extern "C++" simple_window_api
	void setWindowLocation(Window& window, Vector2u& location);

	extern "C++" simple_window_api
	void setWindowProperties(Window& window, Vector2u& location, Vector2u& size);

}