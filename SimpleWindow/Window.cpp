#pragma once
#include "pch.h"
#include "Event.h"
#include "Window.h"

namespace sw {
	static int window_count = 0; // Windows owned by SWL
	const wchar_t* class_name = L"SWM_Window";

	void initWindowZeroParams(Window& window)
	{
		char* title = (char*)malloc(2 * sizeof(char));
		*title = '\0';
		initWindowTwoParams(window, Vector2u{ 1200, 800 }, title);
	}

	void initWindowTwoParams(Window& window, Vector2u size, char* title)
	{
		initWindow(window, Vector2i{ 0, 0 }, size, title);
	}

	void initWindow(Window& window, Vector2i position, Vector2u size, char* title)
	{
		window.position = position;
		window.size = size;
		window.title = title;

		if (window_count == 0)
			createAndRegisterWindowClass(window);
		++window_count;

		window.handle = windowCreate(&window, position, size, title);
		ShowWindow(window.handle, SW_SHOW);

		windowProcessEvents(window);
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
		window_class.lpfnWndProc = windowProcedure;

		return window_class;
	}

	HWND windowCreate(Window* window, Vector2i position, Vector2u size, char* title)
	{
		// convert char* to wchar_t *
		wchar_t* wtitle = (wchar_t*)calloc(strlen(title) + 1, sizeof(wchar_t));
		mbstowcs(wtitle, title, strlen(title));

		return CreateWindowEx(
			0,
			class_name,
			wtitle,
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

	LRESULT CALLBACK windowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (message == WM_NCCREATE)
		{
			auto window = reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*> (lparam)->lpCreateParams);
			SetWindowLongPtrW(handle, GWLP_USERDATA, window);
		}

		Window* window = handle ? reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

		if (window)
		{
			windowProcessEvent(window, message, wparam, lparam);
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

	void windowProcessEvents(Window& window)
	{
		MSG message = { };

		while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void windowProcessEvent(Window* window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (!window->handle) return;
		std::uint32_t character;
		Event event;

		switch (message)
		{
		// Window on close
		case WM_CLOSE:
			event.type = Event::Closed;
			windowPushEvent(window, event);
			break;

		// Window Resize
		case WM_SIZE:
			event.type = Event::Resized;
			event.size.width = (unsigned int)(short)LOWORD(lparam);
			event.size.height = (unsigned int)(short)HIWORD(lparam);
			window->size = Vector2u{ event.size.width, event.size.height };
			windowPushEvent(window, event);
			break;

		case WM_MOVE:
			window->position = Vector2i{
				(int)(short) LOWORD(lparam),
				(int)(short) HIWORD(lparam)
				};
			break;

		case WM_PAINT:
			event.type = Event::Paint;
			windowPushEvent(window, event);
			break;

			// Window get focus
		case WM_SETFOCUS:
			event.type = Event::GainedFocus;
			windowPushEvent(window, event);
			break;

			// Window lost focus
		case WM_KILLFOCUS:
			event.type = Event::LostFocus;
			windowPushEvent(window, event);
			break;

			// Text entere
		case WM_CHAR:
			character = static_cast<std::uint32_t>(wparam);
			event.type = Event::TextEntered;
			event.title.unicodeChar = character; // change to keyCode
			windowPushEvent(window, event);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyPressed;
			event.key.code		= static_cast<unsigned int>(wparam);
			event.key.alt		= HIWORD(GetKeyState(VK_MENU)) != 0;
			event.key.control	= HIWORD(GetKeyState(VK_CONTROL)) != 0;
			event.key.shift		= HIWORD(GetKeyState(VK_SHIFT)) != 0;
			event.key.system	= HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			windowPushEvent(window, event);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyReleased;
			event.key.code		= static_cast<unsigned int>(wparam);
			event.key.alt		= HIWORD(GetKeyState(VK_MENU)) != 0;
			event.key.control	= HIWORD(GetKeyState(VK_CONTROL)) != 0;
			event.key.shift		= HIWORD(GetKeyState(VK_SHIFT)) != 0;
			event.key.system	= HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			windowPushEvent(window, event);
			break;

		case WM_MOUSEMOVE:
			event.type = Event::MouseMoved;
			event.mouseMove.x = GET_X_LPARAM(lparam);
			event.mouseMove.y = GET_Y_LPARAM(lparam);
			windowPushEvent(window, event);
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			event.type = Event::MouseButtonPressed;
			event.mouseClick.code = wparam;
			event.mouseClick.x = GET_X_LPARAM(lparam);
			event.mouseClick.y = GET_Y_LPARAM(lparam);
			windowPushEvent(window, event);
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			event.type = Event::MouseButtonReleased;
			event.mouseClick.code = wparam;
			event.mouseClick.x = GET_X_LPARAM(lparam);
			event.mouseClick.y = GET_Y_LPARAM(lparam);
			windowPushEvent(window, event);
			break;

		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			event.type = Event::MouseWheelScrolled;
			event.mouseScroll.z = GET_WHEEL_DELTA_WPARAM(wparam);
			windowPushEvent(window, event);
			break;

		case WM_NCMOUSELEAVE:
		case WM_MOUSELEAVE:
			event.type = Event::MouseEntereOrLeft;
			windowPushEvent(window, event);
			break;

		case WM_COMMAND:
			if (wparam == WidgetButtonClickEvent)
			{
				event.type = Event::ButtonClick;
				event.button.ID = (HWND)lparam;
				windowPushEvent(window, event);
			}
			break;
		}
	}

	bool windowPollEvent(Window& window, Event& event)
	{
		if (window.events.empty())
		{
			windowProcessEvents(window);

			return false;
		}
		else {
			event = window.events.front();
			window.events.pop();

			return true;
		}
	}

	void windowPushEvent(Window* window, Event& event)
	{
		window->events.push(event);
	}

	void windowAddWidget(Window& window, Widget& widget)
	{
		Vector2u widget_position = widget.position;
		Vector2u widget_size = widget.size;

		char* widget_class_name = widgetGetClassNameByType(widget.type);
		char* widget_title = widget.title;
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

	bool windowIsOpen(Window& window)
	{
		return window.handle != nullptr;
	}

	void windowClose(Window& window)
	{
		window.handle = nullptr;
	}

	void windowSetPositionAndSize(Window& window, Vector2i& position, Vector2u& size) {
		if (!window.handle) return;
		windowSetPosition(window, position);
		windowSetSize(window, size);
	}

	void windowSetPosition(Window& window, Vector2i& position)
	{
		if (!window.handle) return;
		window.position = position;
		SetWindowPos(window.handle, NULL, position.x, position.y, window.size.x, window.size.y, 0);
	}
		
	void windowSetSize(Window& window, Vector2u& size)
	{
		if (!window.handle) return;
		window.size = size;
		SetWindowPos(window.handle, NULL, window.position.x, window.position.y, size.x, size.y, 0);
	}

	HWND getHandle(Window& window)
	{
		return window.handle;
	}
}