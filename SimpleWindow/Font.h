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
		Font() : Font(12, FW_NORMAL, "Times New Roman") { }
		Font(int size, int weight, std::string fontFamily) : Font(TextAlign::Left, size, weight, false, false, false, fontFamily) { }
		Font(TextAlign align, int size, int weight, bool isCursive, bool isUnderline, bool isStrikeOut, std::string fontFamily);

		HFONT getSystemFont();
		TextAlign getAlign();
		int getSize();

	private:
		TextAlign align;
		int size;
		int weight;
		bool isCursive;
		bool isUnderline;
		bool isStrikeOut;
		std::string fontFamily;
		HFONT systemFont;
	};
}