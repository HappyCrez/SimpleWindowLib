#pragma once
#include "pch.h"
#include "Widget.h"


namespace sw {

	void initWidgetZeroParams(Widget& widget)
	{
		char* title = (char*)malloc(2 * sizeof(char));
		*title = '\0';
		initWidgetTwoParams(widget, WidgetType::Label, title);
	}

	void initWidgetTwoParams(Widget& widget, WidgetType type, char* title)
	{
		initWidget(widget, Font(), type, Vector2u{ 200, 50 }, Vector2u{ 0, 0 }, title);
	}

	void initWidget(Widget& widget, Font text_style, WidgetType type, Vector2u size, Vector2u position, char* title)
	{
		widget.handle = nullptr;
		widget.size = size;
		widget.position = position;
		widget.title = title;
		widget.text_style = text_style;
		widget.type = type;
		widget.class_name = widgetGetClassNameByType(type);
	}

	char* widgetGetClassNameByType(WidgetType type) {
		char* widget_class_name = (char*)malloc(10 * sizeof(char));
		switch (type)
		{
		case WidgetType::Button:
			strcpy(widget_class_name, "button\0");
			return widget_class_name;
			break;
		case WidgetType::Label:
			strcpy(widget_class_name, "static\0");
			return widget_class_name;
			break;
		case WidgetType::TextField:
			strcpy(widget_class_name, "edit\0");
			return widget_class_name;
			break;
		default:
			exit(UNDEFINED_WIDGET_TYPE);
		}
	}

	void widgetSetSize(Widget& widget, Vector2u size)
	{
		if (!widget.handle) return;
		widget.size = size;
		SetWindowPos(widget.handle, NULL, widget.position.x, widget.position.y, size.x, size.y, 0);
	}

	void widgetSetPosition(Widget& widget, Vector2u position)
	{
		if (!widget.handle) return;
		widget.position = position;
		SetWindowPos(widget.handle, NULL, position.x, position.y, widget.size.x, widget.size.y, 0);
	}

	void widgetSetText(Widget& widget, char* title)
	{
		if (!widget.handle) return;
		SetWindowTextA(widget.handle, title);
	}

	char* widgetGetText(Widget& widget, int buffer_size)
	{
		if (!widget.handle) return widget.title; // When window doesn't create, return title from constructor
		char* buffer = (char*)std::calloc(buffer_size, sizeof(char));
		GetWindowTextA(widget.handle, buffer, buffer_size);
		return buffer;
	}

	bool buttonIsClicked(Widget& button, Event& event)
	{
		if (event.button.ID == button.handle)
				return true;
		return false;
	}

}