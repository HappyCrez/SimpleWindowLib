#pragma once
#include "pch.h"
#include "Font.h"
#define clickEvent 1

namespace sw {
	enum WidgetType
	{
		Button,
		Label,
		TextField
	};

	class simple_window_api Widget {
	public:
		Widget() : Widget(WidgetType::Label, "") { }
		Widget(WidgetType type, std::string title) : Widget(Font(), type, Vector2u(200, 50), Vector2u(0, 0), title) { }
		Widget(Font textStyle, WidgetType type, Vector2u size, Vector2u position, std::string title);

		void setSize(Vector2u size);
		void setPosition(Vector2u position);
		void setHandle(HWND handle);

		Vector2u getSize();
		Vector2u getPosition();
		std::string getTitle();
		Font getFont();

		HWND getHandle();
		std::string getTypeName();
		int getType();

	private:
		HWND handle;

		WidgetType type;
		std::string class_name;
		
		std::string title;
		Vector2u size;
		Vector2u position;
		Font textStyle;
	};
}