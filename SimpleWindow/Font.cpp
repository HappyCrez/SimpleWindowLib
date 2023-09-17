#pragma once
#include "pch.h"
#include "Font.h"

namespace sw
{
	Font::Font(TextAlign align, int size, std::string fontFamily) {
		Font::align = align;
		Font::size = size;
		Font::fontFamily = fontFamily;
		
		std::wstring wFontFamily(std::begin(fontFamily), std::end(fontFamily));

		Font::systemFont = CreateFont(15, 5, 0, 0,
			FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, &wFontFamily[0]);
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