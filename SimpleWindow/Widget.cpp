#pragma once
#include "pch.h"
#include "Widget.h"


namespace sw {
	Widget::Widget(Font textStyle, WidgetType type, Vector2u size, Vector2u position, std::string text)
	{
		handle = nullptr;
		Widget::size = size;
		Widget::position = position;
		Widget::text = text;
		Widget::textStyle = textStyle;
		Widget::type = type;
		
		switch (type)
		{
		case WidgetType::Button:
			class_name = "button";
			break;
		case WidgetType::Label:
			class_name = "static";
			break;
		case WidgetType::TextField:
			class_name = "edit";
			break;
		}
	}

	void Widget::setSize(Vector2u size)
	{
		if (!handle) return;
		Widget::size = size;
		SetWindowPos(handle, NULL, position.x, position.y, size.x, size.y, 0);
	}

	void Widget::setPosition(Vector2u position)
	{
		if (!handle) return;
		Widget::position = position;
		SetWindowPos(handle, NULL, position.x, position.y, size.x, size.y, 0);
	}

	void Widget::setHandle(HWND handle)
	{
		if (!handle) return;
		Widget::handle = handle;
	}

	void Widget::setText(std::string text)
	{
		if (!handle) return;
		SetWindowTextA(handle, &text[0]);
	}

	std::string Widget::getText(int symbolsCount)
	{
		if (!handle) return text; // When window doesn't create, return title from constructor
		char* buffer = (char*)std::calloc(symbolsCount, sizeof(char));
		GetWindowTextA(handle, buffer, symbolsCount);
		return buffer;
	}

	Vector2u Widget::getPosition()
	{
		return position;
	}

	Vector2u Widget::getSize()
	{
		return size;
	}

	std::string Widget::getTypeName()
	{
		return class_name;
	}

	HWND Widget::getHandle()
	{
		return handle;
	}

	int Widget::getType()
	{
		return type;
	}

	Font Widget::getFont()
	{
		return textStyle;
	}

	bool Widget::isClicked(Event& event)
	{
		if (event.button.ID == handle)
				return true;
		return false;
	}

}