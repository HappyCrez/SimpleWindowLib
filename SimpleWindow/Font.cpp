#pragma once
#include "pch.h"
#include "Font.h"

namespace sw
{
	Font::Font(TextAlign align, int size, int weight, bool is_cursive, bool is_underline, bool is_strikeOut, std::string font_family) {
		Font::align = align;
		Font::size = size;
		Font::font_family = font_family;
		Font::weight = weight;
		Font::is_cursive = is_cursive;
		Font::is_underline = is_underline;
		Font::is_strikeOut = is_strikeOut;

		Font::system_font = CreateFontA(size, 0, 0, 0,
			weight, is_cursive, is_underline, is_strikeOut, OEM_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, &font_family[0]);
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
		return system_font;
	}
}