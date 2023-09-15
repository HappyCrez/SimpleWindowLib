#pragma once
#include "pch.h"
#include "WindowBase.h"
#include "Event.h"

namespace sw {

	const wchar_t* className	= L"SWM_Window";
	static int window_count		= 0;	// Windows owned by SW
	Vector2u lastSize;					// Window lastSize

	void Window::initWindow(Vector2u& location, Vector2u& size, std::string title) {

		if (window_count == 0)
			createAndRegisterWindowClass();
		++window_count;

		Window::m_handle = createWin(location, size, title);

		ShowWindow(m_handle, SW_SHOW);
		processMessages();
	}

	HWND createWin(Vector2u& location, Vector2u& size, std::string title) {
		// convert string to wstring
		std::wstring wTitle(std::begin(title), std::end(title));

		return CreateWindowEx(
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

		Event event;
		switch (msg)
		{
		case WM_CREATE:
			
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		// Window on close
		case WM_CLOSE:
			event.type = Event::Closed;
			pushEvent(event);
			DestroyWindow(hwnd);
			/*if (MessageBox(hwnd, L"Realy Quit??", L"My app", MB_OKCANCEL) == IDOK) {
				DestroyWindow(hwnd);
			}*/
			return 0;

		// Window Resize
		case WM_SIZE:
			event.type = Event::Resized;
			lastSize = getWindowSize(hwnd);
			event.size.width = lastSize.x;
			event.size.height = lastSize.y;
			pushEvent(event);
			return 0;
		
		// Window get focus
		case WM_SETFOCUS:
			event.type = Event::GainedFocus;
			pushEvent(event);
			return 0;
		
		// Window lost focus
		case WM_KILLFOCUS:
			event.type = Event::LostFocus;
			pushEvent(event);
			return 0;
		
		// Text entere
		case WM_CHAR:
			event.type = Event::TextEntered;
			event.key.code = 10; // change to keyCode
			pushEvent(event);
			return 0;
		
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyPressed;
			event.key.code = 10;
			pushEvent(event);
			return 0;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyReleased;
			event.key.code = 10;
			pushEvent(event);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void Window::clear() {
		DestroyWindow(Window::m_handle);
		Window::m_handle = nullptr;
	}

	bool Window::isOpen() {
		return Window::m_handle != nullptr;
	}

	void Window::processMessages() {
		MSG message = { };

		while (GetMessage(&message, NULL, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		Window::clear();
	}

	void Window::setParams(Vector2u& location, Vector2u& size) {
		Window::setLocation(location);
		Window::setSize(size);
	}

	void Window::setLocation(Vector2u& location) {
		SetWindowPos(Window::m_handle, NULL, location.x, location.y, Window::size.x, Window::size.y, 0);
	}

	void Window::setSize(Vector2u& size) {
		SetWindowPos(Window::m_handle, NULL, Window::location.x, Window::location.y, size.x, size.y, 0);
	}

	Vector2u Window::getSize () {
		if (m_handle)
			getWindowSize(Window::m_handle);
	}

	Vector2u getWindowSize(HWND hwnd) {
		RECT rect;
		GetClientRect(hwnd, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));
	}

}