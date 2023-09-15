#pragma once
#include "pch.h"

namespace sw {

	class simple_window_api Window32API {
	public:
		static void createAndRegisterWindowClass();
		static WNDCLASS createWindowClass();
		
		static HWND createWin(Vector2u& location, Vector2u& size, std::string title);
		static LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		
		static Vector2u getWindowSize(HWND hwnd);
	};

}