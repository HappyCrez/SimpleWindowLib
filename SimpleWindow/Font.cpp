#pragma once
#include "pch.h"
#include "Font.h"

namespace sw
{
	void initFontZeroParams(Font& font)
	{
		initFontThreeParams(font, 12, FW_LIGHT, "Times New Roman");
	}

	void initFontThreeParams(Font& font, int size, int weight, std::string fontFamily)
	{
		initFont(font, TextAlign::Left, size, weight, false, false, false, fontFamily);
	}

	void initFont(Font& font, TextAlign align, int size, int weight, bool isCursive, bool isUnderline, bool isStrikeOut, std::string fontFamily)
	{
		font.align = align;
		font.size = size;
		font.fontFamily = fontFamily;
		font.weight = weight;
		font.isCursive = isCursive;
		font.isUnderline = isUnderline;
		font.isStrikeOut = isStrikeOut;

		font.systemFont = CreateFontA(size, 0, 0, 0,
			weight, isCursive, isUnderline, isStrikeOut, OEM_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, &fontFamily[0]);
	}
}