#pragma once
#include "pch.h"
#include "Event.h"
#include "Window.h"

namespace sw {
	static int window_count = 0; // Windows owned by SWL
	Vector2u last_size;	// Window lastSize
	const wchar_t* class_name = L"SWM_Window";

	void initWindowZeroParams(Window& window)
	{
		initWindowTwoParams(window, Vector2u(1200, 800), "Hello world");
	}

	void initWindowTwoParams(Window& window, Vector2u size, std::string title)
	{
		initWindowThreeParams(window, Vector2u(0, 0), size, title);
	}

	void initWindowThreeParams(Window& window, Vector2u position, Vector2u size, std::string title)
	{
		if (window_count == 0)
			createAndRegisterWindowClass(window);
		++window_count;

		window.handle = createWin(&window, position, size, title);
		ShowWindow(window.handle, SW_SHOW);

		processEvents(window);
	}

	void createAndRegisterWindowClass(Window& window)
	{
		WNDCLASS window_class = createWindowClass(window);
		RegisterClassW(&window_class);
	}

	WNDCLASS createWindowClass(Window& window)
	{
		WNDCLASS window_class = { };
		window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		window_class.hInstance = GetModuleHandle(nullptr);
		window_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
		window_class.lpszClassName = class_name;
		window_class.lpfnWndProc = winProcedure;

		return window_class;
	}

	HWND createWin(Window* window, Vector2u position, Vector2u size, std::string title)
	{
		// convert string to wstring and use as wchar_t *
		std::wstring wchar_title(std::begin(title), std::end(title));

		return CreateWindowEx(
			0,
			class_name,
			&wchar_title[0],	
			WS_OVERLAPPEDWINDOW,
			position.x,
			position.y,
			size.x,
			size.y,
			NULL,						
			NULL,						
			GetModuleHandle(nullptr),	
			window
		);
	}

	LRESULT CALLBACK winProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (message == WM_NCCREATE)
		{
			auto window = reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*> (lparam)->lpCreateParams);
			SetWindowLongPtrW(handle, GWLP_USERDATA, window);
		}

		Window* window = handle ? reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

		if (window)
		{
			processEvent(window, message, wparam, lparam);
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

	void processEvents(Window& window)
	{
		MSG message = { };

		while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void processEvent(Window* window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (!window->handle) return;
		std::uint32_t character;
		Event event;

		switch (message)
		{
		// Window on close
		case WM_CLOSE:
			event.type = Event::Closed;
			pushEvent(window, event);
			break;

		// Window Resize
		case WM_SIZE:
			event.type = Event::Resized;
			last_size = window->size;
			event.size.width = last_size.x;
			event.size.height = last_size.y;
			pushEvent(window, event);
			break;

		case WM_PAINT:
			event.type = Event::Paint;
			pushEvent(window, event);
			break;

			// Window get focus
		case WM_SETFOCUS:
			event.type = Event::GainedFocus;
			pushEvent(window, event);
			break;

			// Window lost focus
		case WM_KILLFOCUS:
			event.type = Event::LostFocus;
			pushEvent(window, event);
			break;

			// Text entere
		case WM_CHAR:
			character = static_cast<std::uint32_t>(wparam);
			event.type = Event::TextEntered;
			event.title.unicodeChar = character; // change to keyCode
			pushEvent(window, event);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyPressed;
			event.key.code		= static_cast<unsigned int>(wparam);
			event.key.alt		= HIWORD(GetKeyState(VK_MENU)) != 0;
			event.key.control	= HIWORD(GetKeyState(VK_CONTROL)) != 0;
			event.key.shift		= HIWORD(GetKeyState(VK_SHIFT)) != 0;
			event.key.system	= HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			pushEvent(window, event);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyReleased;
			event.key.code		= static_cast<unsigned int>(wparam);
			event.key.alt		= HIWORD(GetKeyState(VK_MENU)) != 0;
			event.key.control	= HIWORD(GetKeyState(VK_CONTROL)) != 0;
			event.key.shift		= HIWORD(GetKeyState(VK_SHIFT)) != 0;
			event.key.system	= HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			pushEvent(window, event);
			break;

		case WM_MOUSEMOVE:
			event.type = Event::MouseMoved;
			event.mouseMove.x = GET_X_LPARAM(lparam);
			event.mouseMove.y = GET_Y_LPARAM(lparam);
			pushEvent(window, event);
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			event.type = Event::MouseButtonPressed;
			event.mouseClick.code = wparam;
			event.mouseClick.x = GET_X_LPARAM(lparam);
			event.mouseClick.y = GET_Y_LPARAM(lparam);
			pushEvent(window, event);
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			event.type = Event::MouseButtonReleased;
			event.mouseClick.code = wparam;
			event.mouseClick.x = GET_X_LPARAM(lparam);
			event.mouseClick.y = GET_Y_LPARAM(lparam);
			pushEvent(window, event);
			break;

		case WM_MOUSEHWHEEL:
			event.type = Event::MouseWheelScrolled;
			event.mouseScroll.z = GET_WHEEL_DELTA_WPARAM(wparam);
			pushEvent(window, event);
			break;

		case WM_MOUSELEAVE:
			event.type = Event::MouseLeft;
			pushEvent(window, event);
			break;

		case WM_MOUSEHOVER:
			event.type = Event::MouseEntered;
			pushEvent(window, event);
			break;

		case WM_COMMAND:
			if (wparam == WidgetButtonClickEvent)
			{
				event.type = Event::ButtonClick;
				event.button.ID = (HWND)lparam;
				pushEvent(window, event);
			}
			break;
		}
	}

	bool pollEvent(Window& window, Event& event)
	{
		if (window.events.empty())
		{
			processEvents(window);

			return false;
		}
		else {
			event = window.events.front();
			window.events.pop();

			return true;
		}
	}

	void pushEvent(Window* window, Event& event)
	{
		window->events.push(event);
	}

	void add(Window& window, Widget& widget)
	{
		Vector2u widget_position = widget.position;
		Vector2u widget_size = widget.size;

		std::string widget_class_name = widgetGetClassNameByType(widget.type);
		std::string widget_title = widget.title;
		Font widget_font = widget.text_style;
		WidgetType widget_type = widget.type;

		long int flags = getWidgetFlagsByType(widget_type, widget_font);

		HMENU wmCommandFlag = NULL;
		if (widget.type == WidgetType::Button)
		{
			wmCommandFlag = (HMENU)WidgetButtonClickEvent;
		}

		HWND widget_handle = CreateWindowA(
			&widget_class_name[0], &widget_title[0], flags,
			widget_position.x, widget_position.y,
			widget_size.x, widget_size.y, 
			window.handle, wmCommandFlag, NULL, NULL);
		
		// Remove std bounds
		SendMessageA(widget_handle, WM_CHANGEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
		
		// Set Font
		SendMessageA(widget_handle, WM_SETFONT, WPARAM(widget_font.system_font), TRUE);

		widget.handle = widget_handle;
	}

	long int getWidgetFlagsByType(WidgetType widget_type, Font widget_font)
	{
		long int flags = WS_VISIBLE | WS_CHILD;
		switch (widget_font.align)
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

		switch (widget_type)
		{
			case WidgetType::TextField:
				flags |= ES_MULTILINE;
				flags |= WS_VSCROLL;
				break;
			case WidgetType::Button:
				flags |= BS_FLAT;	// Make button in 2D format (3D - std)
				flags |= BS_NOTIFY; // Push FocusEvent to parent window
				break;
		}

		return flags;
	}

	bool isOpen(Window& window)
	{
		return window.handle != nullptr;
	}

	void close(Window& window)
	{
		window.handle = nullptr;
	}

	void setParams(Window& window, Vector2u& position, Vector2u& size) {
		if (!window.handle) return;
		setPosition(window, position);
		setSize(window, size);
	}

	void setPosition(Window& window, Vector2u& position)
	{
		if (!window.handle) return;
		SetWindowPos(window.handle, NULL, position.x, position.y, window.size.x, window.size.y, 0);
	}
		
	void setSize(Window& window, Vector2u& size)
	{
		if (!window.handle) return;
		SetWindowPos(window.handle, NULL, window.position.x, window.position.y, size.x, size.y, 0);
	}

	Vector2u getSize(Window& window)
	{
		if (!window.handle) return Vector2u();
		RECT rect;
		GetClientRect(window.handle, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));;
	}

	HWND getHandle(Window& window)
	{
		return window.handle;
	}
}