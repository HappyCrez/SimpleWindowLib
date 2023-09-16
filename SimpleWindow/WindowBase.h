#pragma once
#include "pch.h"
#include "Event.h"
//
//namespace sw {
//
//	class simple_window_api Window {
//	public:
//		Window () : Window("New Window") { }
//
//		Window(std::string title) : Window(Vector2u(0, 0), Vector2u(300, 300), title) { }
//
//		Window(Vector2u location, Vector2u size, std::string title);
//
//		bool isOpen();
//		void close();
//
//		void setSize(Vector2u& size);
//		void setLocation(Vector2u& location);
//		void setParams(Vector2u& location, Vector2u& size);
//
//		Vector2u getSize();
//		HWND getHandle();	// Client can extend library oportunites and use win32API
//
//		bool pollEvent(Event& event);
//		
//	private:
//		Vector2u location = Vector2u(0, 0);
//		Vector2u size = Vector2u(500, 300);
//		std::string title = "New window";
//		HWND m_handle = nullptr;
//
//		void initWindow(Vector2u& location, Vector2u& size, std::string title);
//		void processEvents();
//	};
//}