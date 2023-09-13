#pragma once
#include "pch.h"
#include "WindowBase.h"
#include "Event.h"

namespace sw {

	const wchar_t* className	= L"SWM_Window";
	static int window_count		= 0;	// Windows owned by SW
	HWND m_impl = nullptr;				// 
	Vector2u lastSize;					// Window lastSize

	HWND initializeWindow() {
		Vector2u location = Vector2u(100, 100);
		Vector2u size = Vector2u(500, 400);
		std::string title = "window title";

		return initializeWindow(location, size, title);
	}

	HWND initializeWindow(Vector2u& location, Vector2u& size, std::string title) {

		// convert string to wstring
		std::wstring wTitle(std::begin(title), std::end(title));

		if (window_count == 0)
			createAndRegisterWindowClass();

		MSG message = { 0 };

		HWND m_handle = CreateWindowEx(
			0,
			className,					
			&wTitle[0],					// wchar_t*
			WS_OVERLAPPEDWINDOW,
			location.x,
			location.y,
			size.x,
			size.y,
			NULL,						// Parent Window
			NULL,						// Menu
			GetModuleHandle(nullptr),	// Instance handle
			NULL						// Additional application data
			);
		ShowWindow(m_handle, SW_SHOW);

		++window_count;

		m_impl = m_handle;

		return m_handle;
	}

	void createAndRegisterWindowClass() {
		WNDCLASS winClass = createWNDCLASS();
		RegisterClassW(&winClass);
	}

	WNDCLASS createWNDCLASS() {
		WNDCLASS winClass = { };
		winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hInstance = GetModuleHandle(nullptr);
		winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		winClass.lpszClassName = className;
		winClass.lpfnWndProc = winProcedure;

		return winClass;
	}

	LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg)
		{
		case WM_CREATE:

			return 0;

		case WM_DESTROY:
			m_impl = nullptr;
			PostQuitMessage(0);
			return 0;

		// Window on close
		case WM_CLOSE:

			if (MessageBox(hwnd, L"Realy Quit??", L"My app", MB_OKCANCEL) == IDOK) {
				DestroyWindow(hwnd);
			}
			return 0;

		// Window Resize
		case WM_SIZE:
			Event event;
			event.type = Event::Resized;
			lastSize = getWindowSize(hwnd);
			event.size.width = lastSize.x;
			event.size.height = lastSize.y;
			pushEvent(event);
			return 0;
		
		// Window get focus
		case WM_SETFOCUS:

			return 0;
		
		// Window lost focus
		case WM_KILLFOCUS:

			return 0;
		
		// Text entere
		case WM_CHAR:

			return 0;
		
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:

			return 0;

		case WM_KEYUP:
		case WM_SYSKEYUP:

			return 0;
		}
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	bool isWindowOpen(Window& window) {
		return m_impl != nullptr;
	}

	// TODO::for some Time, RENAME!!!
	void translateMessagesSomeTime(Window& window) {
		MSG message = { };
		while (GetMessage(&message, NULL, 0, 0) != 0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		m_impl = nullptr; // Then window closed
	}

	void setWindowProperties(Window& window, Vector2u& location, Vector2u& size) {
		setWindowLocation(window, location);
		setWindowSize(window, size);
	}

	void setWindowLocation(Window& window, Vector2u& location) {
		SetWindowPos(window.m_handle, NULL, location.x, location.y, window.size.x, window.size.y, 0);
	}

	void setWindowSize(Window& window, Vector2u& size) {
		SetWindowPos(window.m_handle, NULL, window.location.x, window.location.y, size.x, size.y, 0);
	}

	Vector2u getWindowSize(HWND hwnd) {
		RECT rect;
		GetClientRect(hwnd, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));
	}
}