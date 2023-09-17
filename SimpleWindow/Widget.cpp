#pragma once
#include "pch.h"
#include "Widget.h"

namespace sw {
	Widget::Widget(Font textStyle, WidgetType type, Vector2u size, Vector2u position, std::string title)
	{
		handle = nullptr;
		Widget::size = size;
		Widget::position = position;
		Widget::title = title;
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
		Widget::size = size;
		if (!handle) return;
		SetWindowPos(handle, NULL, position.x, position.y, size.x, size.y, 0);
	}

	void Widget::setPosition(Vector2u position)
	{
		Widget::position = position;
		if (!handle) return;
		SetWindowPos(handle, NULL, position.x, position.y, size.x, size.y, 0);
	}

	void Widget::setHandle(HWND handle)
	{
		Widget::handle = handle;
	}

	Vector2u Widget::getPosition()
	{
		return position;
	}

	Vector2u Widget::getSize()
	{
		return size;
	}

	std::string Widget::getTitle()
	{
		return title;
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

	/*LRESULT CALLBACK Widget::processWidget(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		std::cout << "true\n";


		return CallWindowProc(lastProc, handle, message, wparam, lparam);
	}*/
}