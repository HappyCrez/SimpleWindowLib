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
		Font() : Font(TextAlign::Center, 12, "Times New Roman") { }
		Font(TextAlign align, int size, std::string fontFamily);

		HFONT getSystemFont();
		TextAlign getAlign();
		int getSize();

	private:
		TextAlign align;
		int size;
		std::string fontFamily;
		HFONT systemFont;
	};
}