#include "pch.h"
#include "SWM.h"


namespace sw {

	const wchar_t* className	= L"SWM_Window";
	static int window_count		= 0;	// Windows owned by SW
	HWND m_impl = nullptr;				// 

	HWND initializeWindow() {
		Vector2u location = Vector2u(100, 100);
		Vector2u size = Vector2u(500, 400);
		std::string title = "window title";

		return initializeWindow(location, size, title);
	}

	HWND initializeWindow(Vector2u& location, Vector2u& size, std::string title) {

		// convert string to wstring
		std::wstring wTitle(std::begin(title), std::end(title));

		if (window_count == 0)
			createAndRegisterWindowClass();

		MSG message = { 0 };

		HWND m_handle = CreateWindow(
			className,					
			&wTitle[0],					// wchar_t*
			WS_OVERLAPPED | WS_VISIBLE,
			location.x,
			location.y,
			size.x,
			size.y,
			NULL,						// Parent Window
			NULL,						// Menu
			GetModuleHandle(nullptr),	// Instance handle
			NULL						// Additional application data
			);

		++window_count;

		m_impl = m_handle;

		return m_handle;
	}

	void createAndRegisterWindowClass() {
		WNDCLASS winClass = createWNDCLASS();
		RegisterClassW(&winClass);
	}


	WNDCLASS createWNDCLASS() {
		WNDCLASS winClass = { 0 };
		winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hInstance = GetModuleHandle(nullptr);
		winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		winClass.lpszClassName = className;
		winClass.lpfnWndProc = winProcedure;

		return winClass;
	}

	LRESULT CALLBACK winProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg)
		{
		case WM_CREATE:

			break;
		case WM_DESTROY:
			m_impl = nullptr;
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	bool isWindowOpen(Window& window) {
		return m_impl != nullptr;
	}

	void pollEvent(Window& window) {
		MSG message = { };
		while (GetMessage(&message, NULL, 0, 0) != 0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		m_impl = nullptr; // Then window closed
	}

	void setWindowProperties(Window& window, Vector2u& location, Vector2u& size) {
		setWindowLocation(window, location);
		setWindowSize(window, size);
	}

	void setWindowLocation(Window& window, Vector2u& location) {
		SetWindowPos(window.m_handle, NULL, location.x, location.y, window.size.x, window.size.y, 0);
	}

	void setWindowSize(Window& window, Vector2u& size) {
		SetWindowPos(window.m_handle, NULL, window.location.x, window.location.y, size.x, size.y, 0);
	}
}