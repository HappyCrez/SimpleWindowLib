#pragma once
#include "pch.h"
#include "Font.h"
#include "Event.h"

#define WidgetButtonClickEvent 1

namespace sw {

	enum WidgetType
	{
		Button,
		Label,
		TextField
	};

	struct simple_window_api Widget
	{
		HWND handle;

		std::string class_name;
		WidgetType type;
		
		std::string title;
		Font text_style;
		Vector2u size;
		Vector2u position;
	};

	extern "C" simple_window_api
	void initWidgetZeroParams(Widget& widget);
	extern "C" simple_window_api
	void initWidgetTwoParams(Widget& widget, WidgetType type, std::string title);
	extern "C" simple_window_api
	void initWidget(Widget& widget, Font text_style, WidgetType type, Vector2u size, Vector2u position, std::string title);
	extern "C" simple_window_api
	bool buttonIsClicked(Widget& button, Event& event);

	extern "C" simple_window_api
	void widgetSetSize(Widget& widget, Vector2u size);
	extern "C" simple_window_api
	void widgetSetPosition(Widget& widget, Vector2u position);

	extern "C" simple_window_api
	void widgetSetText(Widget& widget, std::string title);
	extern "C" simple_window_api
	std::string widgetGetText(Widget& widget, int buffer_size);

	extern "C" simple_window_api
	std::string widgetGetClassNameByType(WidgetType type);
}