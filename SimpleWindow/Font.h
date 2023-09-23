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

	class simple_window_api Font
	{
	public:
		Font() : Font(12, FW_LIGHT, "Times New Roman") { }
		Font(int size, int weight, std::string font_family) : Font(TextAlign::Left, size, weight, false, false, false, font_family) { }
		Font(TextAlign align, int size, int weight, bool is_cursive, bool is_underline, bool is_strikeOut, std::string font_family);

		HFONT getSystemFont();
		TextAlign getAlign();
		int getSize();

	private:
		TextAlign align;
		int size;
		int weight;
		bool is_cursive;
		bool is_underline;
		bool is_strikeOut;
		std::string font_family;
		HFONT system_font;
	};
}