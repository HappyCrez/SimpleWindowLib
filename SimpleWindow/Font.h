#pragma once
#include "pch.h"
#include <Windows.h>

namespace sw {
	
	enum TextAlign
	{
		Center,
		Left,
		Right
	};

	struct simple_window_api Font
	{
		TextAlign align;
		int size;
		int weight;
		bool is_cursive;
		bool is_underline;
		bool is_strikeOut;
		char* font_family;
		HFONT system_font;
	};

	extern "C" simple_window_api
	void initFontZeroParams(Font& font);
	extern "C" simple_window_api
	void initFontThreeParams(Font& font, int size, int weight, char* font_family);
	extern "C" simple_window_api
	void initFont(Font& font, TextAlign align, int size, int weight, bool is_cursive, bool is_underline, bool is_strikeOut, char* font_family);
}