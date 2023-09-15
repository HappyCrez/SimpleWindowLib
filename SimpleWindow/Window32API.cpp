#pragma once
#include "pch.h"
#include "Event.h"
#include "Window32API.h"

namespace sw {

	Vector2u lastSize;					// Window lastSize
	const wchar_t* className = L"SWM_Window";

	HWND Window32API::createWin(Vector2u& location, Vector2u& size, std::string title) {
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
			NULL						
		);
	}

	void Window32API::createAndRegisterWindowClass() {
		WNDCLASS winClass = Window32API::createWindowClass();
		RegisterClassW(&winClass);
	}

	WNDCLASS Window32API::createWindowClass() {
		WNDCLASS winClass = { };
		winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hInstance = GetModuleHandle(nullptr);
		winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		winClass.lpszClassName = className;
		winClass.lpfnWndProc = Window32API::winProcedure;

		return winClass;
	}

	LRESULT CALLBACK Window32API::winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

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
			event.key.code = wp; // change to keyCode
			pushEvent(event);
			return 0;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyPressed;
			event.key.code = wp;
			pushEvent(event);
			return 0;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyReleased;
			event.key.code = wp;
			pushEvent(event);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	Vector2u Window32API::getWindowSize(HWND hwnd) {
		RECT rect;
		GetClientRect(hwnd, &rect);
		return Vector2u(static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top));
	}
}