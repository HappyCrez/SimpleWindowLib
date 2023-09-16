#pragma once
#include "pch.h"
#include "Event.h"
#include "Window32API.h"

namespace sw {
	static int window_count = 0;	// Windows owned by SWL

	Vector2u lastSize;					// Window lastSize
	const wchar_t* className = L"SWM_Window";

	Window32API::Window32API(Vector2u& location, Vector2u& size, std::string* title) {
		if (window_count == 0)
			createAndRegisterWindowClass();
		++window_count;

		window_handle = createWin(location, size, title);
		ShowWindow(window_handle, SW_SHOW);

		processEvents();
	}

	void Window32API::createAndRegisterWindowClass() {
		WNDCLASS winClass = Window32API::createWindowClass();
		RegisterClassW(&winClass);
	}

	WNDCLASS Window32API::createWindowClass() {
		WNDCLASS winClass = { };
		winClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		winClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		winClass.hInstance		= GetModuleHandle(nullptr);
		winClass.hbrBackground	= (HBRUSH)COLOR_WINDOW;
		winClass.lpszClassName	= className;
		winClass.lpfnWndProc	= winProcedure;

		return winClass;
	}

	HWND Window32API::createWin(Vector2u& location, Vector2u& size, std::string* title) {
		// convert string to wstring and use as wchar_t *
		std::wstring wTitle(std::begin(*title), std::end(*title));

		return CreateWindowEx(
			0,
			className,
			&wTitle[0],	
			WS_OVERLAPPEDWINDOW,
			location.x,
			location.y,
			size.x,
			size.y,
			NULL,						
			NULL,						
			GetModuleHandle(nullptr),	
			this						
		);
	}

	LRESULT CALLBACK Window32API::winProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (message == WM_NCCREATE)
		{
			auto window = reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*> (lparam)->lpCreateParams);
			std::cout << window;
			SetWindowLongPtrW(handle, GWLP_USERDATA, window);
		}

		Window32API* window = handle ? reinterpret_cast<Window32API*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

		if (window)
		{
			window->processEvent(message, wparam, lparam);
		}

		if (message == WM_DESTROY)
		{
			// Cleanup resources
			PostQuitMessage(0);
			return 0;
		}

		if (message == WM_CLOSE)
		{
			// Intercept for defend from auto destroying
			return 0;
		}

		// Save focus on window on alt or F10 click
		if ((message == WM_SYSCOMMAND) && (wparam == SC_KEYMENU))
			return 0;

		return DefWindowProc(handle, message, wparam, lparam);
	}

	void Window32API::processEvent(UINT message, WPARAM wparam, LPARAM lparam)
	{
		Event event;
		switch (message) {

		// Window on close
		case WM_CLOSE:
			event.type = Event::Closed;
			pushEvent(event);
			break;
			// Window Resize
		case WM_SIZE:
			event.type = Event::Resized;
			lastSize = getWindowSize(window_handle);
			event.size.width = lastSize.x;
			event.size.height = lastSize.y;
			pushEvent(event);
			break;

			// Window get focus
		case WM_SETFOCUS:
			event.type = Event::GainedFocus;
			pushEvent(event);
			break;

			// Window lost focus
		case WM_KILLFOCUS:
			event.type = Event::LostFocus;
			pushEvent(event);
			break;

			// Text entere
		case WM_CHAR:
			event.type = Event::TextEntered;
			event.key.code = wparam; // change to keyCode
			pushEvent(event);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyPressed;
			event.key.code = wparam;
			pushEvent(event);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyReleased;
			event.key.code = wparam;
			pushEvent(event);
			break;
		}
	}

	bool Window32API::pollEvent(Event& event) {

		if (event_queue.empty())
		{
			processEvents();

			return false;
		}
		else {
			event = event_queue.front();
			event_queue.pop();

			return true;
		}
	}

	void Window32API::processEvents() {

		MSG message = { };

		while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void Window32API::pushEvent(Event& event) {
		event_queue.push(event);
	}

	Vector2u Window32API::getWindowSize(HWND hwnd) {
		RECT rect;
		GetClientRect(hwnd, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));
	}

	bool Window32API::isOpen() {
		return window_handle != nullptr;
	}
}