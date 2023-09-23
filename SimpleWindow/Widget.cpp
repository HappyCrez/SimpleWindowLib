#pragma once
#include "pch.h"
#include "Widget.h"


namespace sw {

	void initWidgetZeroParams(Widget& widget)
	{
		initWidgetTwoParams(widget, WidgetType::Label, "");
	}

	void initWidgetTwoParams(Widget& widget, WidgetType type, std::string title)
	{
		initWidget(widget, Font(), type, Vector2u(200, 50), Vector2u(0, 0), title);
	}

	void initWidget(Widget& widget, Font textStyle, WidgetType type, Vector2u size, Vector2u position, std::string title)
	{
		widget.handle = nullptr;
		widget.size = size;
		widget.position = position;
		widget.title = title;
		widget.textStyle = textStyle;
		widget.type = type;
		widget.class_name = widgetGetClassNameByType(type);
	}

	std::string widgetGetClassNameByType(WidgetType type) {
		switch (type)
		{
		case WidgetType::Button:
			return "button";
			break;
		case WidgetType::Label:
			return "static";
			break;
		case WidgetType::TextField:
			return "edit";
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

	void widgetSetText(Widget& widget, std::string title)
	{
		if (!widget.handle) return;
		SetWindowTextA(widget.handle, &title[0]);
	}

	std::string widgetGetText(Widget& widget, int symbolsCount)
	{
		if (!widget.handle) return widget.title; // When window doesn't create, return title from constructor
		char* buffer = (char*)std::calloc(symbolsCount, sizeof(char));
		GetWindowTextA(widget.handle, buffer, symbolsCount);
		return buffer;
	}

	bool widgetIsClicked(Widget& widget, Event& event)
	{
		if (event.button.ID == widget.handle)
				return true;
		return false;
	}

}