#pragma once
#include "pch.h"
#include "Event.h"
#include "Window.h"

namespace sw {
	static int window_count = 0;	// Windows owned by SWL
	Vector2u last_size;					// Window lastSize
	const wchar_t* class_name = L"SWM_Window";

	Window::Window(Vector2u position, Vector2u size, std::string title)
	{
		if (window_count == 0)
			createAndRegisterWindowClass();
		++window_count;

		handle = createWin(position, size, title);
		ShowWindow(handle, SW_SHOW);

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
		winClass.lpszClassName	= class_name;
		winClass.lpfnWndProc	= winProcedure;

		return winClass;
	}

	HWND Window::createWin(Vector2u position, Vector2u size, std::string title)
	{
		// convert string to wstring and use as wchar_t *
		std::wstring wTitle(std::begin(title), std::end(title));

		return CreateWindowEx(
			0,
			class_name,
			&wTitle[0],	
			WS_OVERLAPPEDWINDOW,
			position.x,
			position.y,
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
		if (!handle) return;
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
			last_size = getSize();
			event.size.width = last_size.x;
			event.size.height = last_size.y;
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

		case WM_MOUSEMOVE:
			event.type = Event::MouseMoved;
			event.mouse.x = GET_X_LPARAM(lparam);
			event.mouse.y = GET_Y_LPARAM(lparam);
			pushEvent(event);
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			event.type = Event::MouseButtonPressed;
			event.mouseClick.code = wparam;
			event.mouseClick.x = GET_X_LPARAM(lparam);
			event.mouseClick.y = GET_Y_LPARAM(lparam);
			pushEvent(event);
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			event.type = Event::MouseButtonReleased;
			event.mouseClick.code = wparam;
			event.mouseClick.x = GET_X_LPARAM(lparam);
			event.mouseClick.y = GET_Y_LPARAM(lparam);
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

	void Window::add(Widget& widget)
	{
		Vector2u position = widget.getPosition();
		Vector2u size = widget.getSize();

		std::string widget_class_name = widget.getTypeName();
		std::string widget_title = widget.getTitle();

		long int flags = WS_VISIBLE | WS_CHILD;
		switch (widget.getType())
		{
		case WidgetType::Button:

			break;

		case WidgetType::Label:
			break;

		case WidgetType::TextField:
			flags |= ES_MULTILINE;
			flags |= WS_VSCROLL;
			break;
		}

		switch (widget.getFont().getAlign())
		{
		case TextAlign::Center:
			flags |= ES_CENTER;
			break;
		case TextAlign::Left:
			flags |= ES_LEFT;
			break;
		case TextAlign::Right:
			flags |= ES_RIGHT;
			break;
		}

		HWND widget_handle = CreateWindowA(
			&widget_class_name[0], &widget_title[0], flags,
			position.x, position.y,
			size.x, size.y, 
			Window::handle, NULL, NULL, NULL);
		
		
		if (widget.getType() == WidgetType::TextField) SendMessageA(widget_handle, WM_SETFONT, WPARAM(widget.getFont().getSystemFont()), TRUE);
		widget.setHandle(widget_handle);
	}

	bool Window::isOpen()
	{
		return handle != nullptr;
	}

	void Window::close()
	{
		handle = nullptr;
	}

	void Window::setParams(Vector2u& position, Vector2u& size) {
		if (!handle) return;
		setPosition(position);
		setSize(size);
	}

	void Window::setPosition(Vector2u& position)
	{
		if (!handle) return;
		SetWindowPos(handle, NULL, position.x, position.y, size.x, size.y, 0);
	}
		
	void Window::setSize(Vector2u& size)
	{
		if (!handle) return;
		SetWindowPos(handle, NULL, position.x, position.y, size.x, size.y, 0);
	}

	Vector2u Window::getSize()
	{
		if (!handle) return Vector2u();
		RECT rect;
		GetClientRect(handle, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));;
	}

	HWND Window::getHandle()
	{
		return handle;
	}
}