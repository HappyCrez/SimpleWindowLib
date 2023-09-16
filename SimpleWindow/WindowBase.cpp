#pragma once
#include "pch.h"
#include "WindowBase.h"

//namespace sw {
//
//
//	Window::Window (Vector2u location, Vector2u size, std::string title) {
//			Window::location = location;
//			Window::size = size;
//			Window::title = title;
//			m_handle = nullptr;
//
//			// Launch another thread for free main thread
//			std::thread processWindow(
//				std::bind(&Window::initWindow, this,
//				location, size, title)
//				);
//			processWindow.detach();
//
//			// Force main thread to wait window creation
//			while (!Window::m_handle) {
//				Sleep(1000); // 1 second
//			}
//	}
//
//	void Window::initWindow(Vector2u& location, Vector2u& size, std::string title) {
//
//		if (window_count == 0)
//			winImpl.createAndRegisterWindowClass();
//		++window_count;
//
//		m_handle = winImpl.createWin(location, size, title);
//
//		ShowWindow(Window::m_handle, SW_SHOW);
//		processEvents();
//	}
//
//	
//
//	void Window::close() {
//		if (m_handle)
//			DestroyWindow(m_handle);
//		m_handle = nullptr;
//	}
//
//	bool Window::isOpen() {
//		return m_handle != nullptr;
//	}
//
//	void Window::setParams(Vector2u& location, Vector2u& size) {
//		if (!m_handle) return;
//		setLocation(location);
//		setSize(size);
//	}
//
//	void Window::setLocation(Vector2u& location) {
//		if (!m_handle) return;
//		SetWindowPos(m_handle, NULL, location.x, location.y, Window::size.x, Window::size.y, 0);
//	}
//
//	void Window::setSize(Vector2u& size) {
//		if (!m_handle) return;
//		SetWindowPos(m_handle, NULL, Window::location.x, Window::location.y, size.x, size.y, 0);
//	}
//
//	Vector2u Window::getSize () {
//		if (!m_handle) return Vector2u();
//		return winImpl.getWindowSize(Window::m_handle);
//	}
//
//	HWND Window::getHandle() {
//		return m_handle;
//	}
//}