#pragma once
#include "pch.h"
#include "Font.h"
#include "Event.h"

#define WidgetClickEvent 1

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
		Widget(WidgetType type, std::string text) : Widget(Font(), type, Vector2u(200, 50), Vector2u(0, 0), text) { }
		Widget(Font text_style, WidgetType type, Vector2u size, Vector2u position, std::string text);
		
		bool isClicked(Event& event);

		void setSize(Vector2u size);
		void setPosition(Vector2u position);
		void setHandle(HWND handle);
		void setText(std::string text);

		std::string getText(int symbolsCount);
		Vector2u getSize();
		Vector2u getPosition();
		Font getFont();

		HWND getHandle();
		std::string getTypeName();
		int getType();

	private:
		HWND handle;

		WidgetType type;
		std::string class_name;
		
		std::string text;
		Vector2u size;
		Vector2u position;
		Font text_style;
	};
}