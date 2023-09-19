#pragma once
#include "pch.h"
#include "Font.h"

namespace sw
{
	Font::Font(TextAlign align, int size, int weight, bool isCursive, bool isUnderline, bool isStrikeOut, std::string fontFamily) {
		Font::align = align;
		Font::size = size;
		Font::fontFamily = fontFamily;
		Font::weight = weight;
		Font::isCursive = isCursive;
		Font::isUnderline = isUnderline;
		Font::isStrikeOut = isStrikeOut;

		Font::systemFont = CreateFontA(size, 0, 0, 0,
			weight, isCursive, isUnderline, isStrikeOut, OEM_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, &fontFamily[0]);
	}

	TextAlign Font::getAlign()
	{
		return align;
	}

	int Font::getSize()
	{
		return size;
	}

	HFONT Font::getSystemFont() {
		return systemFont;
	}
}