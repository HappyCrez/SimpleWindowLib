#include "pch.h"
#include "Window.h"

void Window::setTitle(std::string winTitle)
{
	std::cout << Window::title << std::endl;
	Window::title = winTitle;
	std::cout << Window::title << std::endl;
}

void Window::set_visible(bool visible)
{
	std::cout << Window::visible;
	Window::visible = visible;
	std::cout << Window::visible;
}
