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
		bool isCursive;
		bool isUnderline;
		bool isStrikeOut;
		std::string fontFamily;
		HFONT systemFont;
	};


	extern "C" simple_window_api
	void initFontZeroParams(Font& font);
	extern "C" simple_window_api
	void initFontThreeParams(Font& font, int size, int weight, std::string fontFamily);
	extern "C" simple_window_api
	void initFont(Font& font, TextAlign align, int size, int weight, bool isCursive, bool isUnderline, bool isStrikeOut, std::string fontFamily);
}