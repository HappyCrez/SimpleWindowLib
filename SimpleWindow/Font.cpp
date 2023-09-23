#pragma once
#include "pch.h"
#include "Font.h"

namespace sw
{
	void initFontZeroParams(Font& font)
	{

		char* font_family = (char*)malloc(20 * sizeof(char));
		strcpy(font_family, "Times New Roman\0");
		initFontThreeParams(font, 12, FW_LIGHT, font_family);
	}

	void initFontThreeParams(Font& font, int size, int weight, char* font_family)
	{
		initFont(font, TextAlign::Left, size, weight, false, false, false, font_family);
	}

	void initFont(Font& font, TextAlign align, int size, int weight, bool is_cursive, bool is_underline, bool is_strikeOut, char* font_family)
	{
		font.align = align;
		font.size = size;
		font.font_family = font_family;
		font.weight = weight;
		font.is_cursive = is_cursive;
		font.is_underline = is_underline;
		font.is_strikeOut = is_strikeOut;

		font.system_font = CreateFontA(size, 0, 0, 0,
			weight, is_cursive, is_underline, is_strikeOut, OEM_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, &font_family[0]);
	}
}