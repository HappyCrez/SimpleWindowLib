#pragma once
#include "pch.h"
#include "Event.h"
#include "Window.h"

namespace sw {
	static int window_count = 0;	// Windows owned by SWL

	Vector2u lastSize;					// Window lastSize
	const wchar_t* className = L"SWM_Window";

	Window::Window(Vector2u location, Vector2u size, std::string title)
	{
		if (window_count == 0)
			createAndRegisterWindowClass();
		++window_count;

		w_handle = createWin(location, size, title);
		ShowWindow(w_handle, SW_SHOW);

		processEvents();
	}

	void Window::createAndRegisterWindowClass()
	{
		WNDCLASS winClass = Window::createWindowClass();
		RegisterClassW(&winClass);
	}

	WNDCLASS Window::createWindowClass()
	{
		WNDCLASS winClass = { };
		winClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		winClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		winClass.hInstance		= GetModuleHandle(nullptr);
		winClass.hbrBackground	= (HBRUSH)COLOR_WINDOW;
		winClass.lpszClassName	= className;
		winClass.lpfnWndProc	= winProcedure;

		return winClass;
	}

	HWND Window::createWin(Vector2u location, Vector2u size, std::string title)
	{
		// convert string to wstring and use as wchar_t *
		std::wstring wTitle(std::begin(title), std::end(title));

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

	LRESULT CALLBACK Window::winProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (message == WM_NCCREATE)
		{
			auto window = reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*> (lparam)->lpCreateParams);
			SetWindowLongPtrW(handle, GWLP_USERDATA, window);
		}

		Window* window = handle ? reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

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
		
		if (!handle) return 0;
		return DefWindowProc(handle, message, wparam, lparam);
	}

	void Window::processEvent(UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (!w_handle) return;
		std::uint32_t character;
		Event event;

		switch (message)
		{

		// Window on close
		case WM_CLOSE:
			event.type = Event::Closed;
			pushEvent(event);
			break;
			// Window Resize
		case WM_SIZE:
			event.type = Event::Resized;
			lastSize = getSize();
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
			character = static_cast<std::uint32_t>(wparam);
			event.type = Event::TextEntered;
			event.text.unicodeChar = character; // change to keyCode
			pushEvent(event);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyPressed;
			event.key.code		= static_cast<unsigned int>(wparam);
			event.key.alt		= HIWORD(GetKeyState(VK_MENU)) != 0;
			event.key.control	= HIWORD(GetKeyState(VK_CONTROL)) != 0;
			event.key.shift		= HIWORD(GetKeyState(VK_SHIFT)) != 0;
			event.key.system	= HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			pushEvent(event);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyReleased;
			event.key.code		= static_cast<unsigned int>(wparam);
			event.key.alt		= HIWORD(GetKeyState(VK_MENU)) != 0;
			event.key.control	= HIWORD(GetKeyState(VK_CONTROL)) != 0;
			event.key.shift		= HIWORD(GetKeyState(VK_SHIFT)) != 0;
			event.key.system	= HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			pushEvent(event);
			break;
		}
	}

	bool Window::pollEvent(Event& event)
	{
		if (events.empty())
		{
			processEvents();

			return false;
		}
		else {
			event = events.front();
			events.pop();

			return true;
		}
	}

	void Window::processEvents()
	{
		MSG message = { };

		while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void Window::pushEvent(Event& event)
	{
		events.push(event);
	}

	bool Window::isOpen()
	{
		return w_handle != nullptr;
	}

	void Window::close()
	{
		w_handle = nullptr;
	}

	void Window::setParams(Vector2u& location, Vector2u& size) {
		if (!w_handle) return;
		setLocation(location);
		setSize(size);
	}

	void Window::setLocation(Vector2u& location)
	{
		if (!w_handle) return;
		SetWindowPos(w_handle, NULL, location.x, location.y, size.x, size.y, 0);
	}
		
	void Window::setSize(Vector2u& size)
	{
		if (!w_handle) return;
		SetWindowPos(w_handle, NULL, location.x, location.y, size.x, size.y, 0);
	}

	Vector2u Window::getSize()
	{
		if (!w_handle) return Vector2u();
		RECT rect;
		GetClientRect(w_handle, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));;
	}

	HWND Window::getHandle()
	{
		return w_handle;
	}
}