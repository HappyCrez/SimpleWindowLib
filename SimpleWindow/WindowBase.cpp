#pragma once
#include "pch.h"
#include "WindowBase.h"
#include "Window32API.h"

namespace sw {

	static int window_count		= 0;	// Windows owned by SWL

	Window::Window (Vector2u location, Vector2u size, std::string title) {
			Window::location = location;
			Window::size = size;
			Window::title = title;
			m_handle = nullptr;

			// Launch another thread for free main thread
			std::thread processWindow(
				std::bind(&Window::initWindow, this,
				location, size, title)
				);
			processWindow.detach();

			// Force main thread to wait window creation
			while (!Window::m_handle) {
				Sleep(1000); // 1 second
			}
	}

	void Window::initWindow(Vector2u& location, Vector2u& size, std::string title) {

		if (window_count == 0)
			Window32API::createAndRegisterWindowClass();
		++window_count;

		Window::m_handle = Window32API::createWin(location, size, title);

		ShowWindow(Window::m_handle, SW_SHOW);
		processMessages();
	}

	void Window::processMessages() {
		MSG message = { };

		while (GetMessage(&message, NULL, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		Window::close();
	}

	void Window::close() {
		if (Window::m_handle)
			DestroyWindow(Window::m_handle);
		Window::m_handle = nullptr;
	}

	bool Window::isOpen() {
		return Window::m_handle != nullptr;
	}

	void Window::setParams(Vector2u& location, Vector2u& size) {
		if (!Window::m_handle) return;
		Window::setLocation(location);
		Window::setSize(size);
	}

	void Window::setLocation(Vector2u& location) {
		if (!Window::m_handle) return;
		SetWindowPos(Window::m_handle, NULL, location.x, location.y, Window::size.x, Window::size.y, 0);
	}

	void Window::setSize(Vector2u& size) {
		if (!Window::m_handle) return;
		SetWindowPos(Window::m_handle, NULL, Window::location.x, Window::location.y, size.x, size.y, 0);
	}

	Vector2u Window::getSize () {
		if (!Window::m_handle) return Vector2u();
		return Window32API::getWindowSize(Window::m_handle);
	}

	HWND Window::getHandle() {
		return Window::m_handle;
	}
}