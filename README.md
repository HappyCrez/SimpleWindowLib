Пример использования библиотеки на структурах C. Совмещенный с использованием стандартного win32api

	#include <SWL.h>

	#include <Commdlg.h>
	#include <fileapi.h>

	int main()
	{
		sw::Vector2u win_size{700, 500};
		sw::Vector2i win_locate{600, 300};
		char win_title [10] = "My title\0";

		sw::Vector2u btn_size{200, 50};

		sw::Font font_courier;
		char str_font_courier[17] = "Courier New\0";
		sw::initFontThreeParams(font_courier, 20, 400, str_font_courier);

		sw::Font label_font;
		sw::initFont(label_font, sw::TextAlign::Center, 30, 500, false, false, false, str_font_courier);

		sw::Font text_field_font;
		sw::initFont(text_field_font, sw::TextAlign::Left, 23, 300, false, false, false, str_font_courier);

		sw::Window window;
		sw::initWindow(window, win_locate, win_size, win_title);
		
		int click_count = 0;
		sw::Widget btn_clear;
		char btn_clear_title[7] = "clear\0";
		sw::initWidget(btn_clear, font_courier, sw::WidgetType::Button, btn_size, sw::Vector2u{win_size.x / 2 - btn_size.x / 2 + 130, win_size.y - btn_size.y - 100}, btn_clear_title);
		sw::windowAddWidget(window, btn_clear);
		
		sw::Widget btn_save;
		char btn_save_title[7] = "save\0";
		sw::initWidget(btn_save, font_courier, sw::WidgetType::Button, btn_size, sw::Vector2u{win_size.x / 2 - btn_size.x / 2 - 130, win_size.y - btn_size.y - 100}, btn_save_title);
		sw::windowAddWidget(window, btn_save);

		sw::Widget label;
		char label_title[27] = "It's my Blacknote, on SWM\0";
		sw::initWidget(label, label_font, sw::WidgetType::Label, sw::Vector2u{win_size.x, 45}, sw::Vector2u{0, 20}, label_title);
		sw::windowAddWidget(window, label);

		sw::Widget text_field;
		char text_field_title[27] = "Redact me\0";
		sw::initWidget(text_field, text_field_font, sw::WidgetType::TextField, sw::Vector2u{500, 200}, sw::Vector2u{win_size.x / 2 - 240, 80}, text_field_title);
		sw::windowAddWidget(window, text_field);

		// block::win32api
		char filename[256] = "myNote.txt";
		OPENFILENAMEA ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = window.handle;
		ofn.lpstrFile = filename;
		ofn.nMaxFile = sizeof(filename);
		ofn.lpstrFilter = ".txt";
		ofn.lpstrTitle = NULL;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		PAINTSTRUCT ps;
		HBRUSH brush = HBRUSH(RGB(200, 13, 123));
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 100;
		rect.bottom = 100;
		// endblock::win32api

		char empty[2] = "\0";
		while (sw::windowIsOpen(window))
		{
			sw::Event event;
			while (sw::windowPollEvent(window, event))
			{
				switch (event.type)
				{
				case sw::Event::Closed:
					sw::windowClose(window);
					break;
				case sw::Event::KeyPressed:
					std::cout << char(event.key.code)<< "\n";
					break;
				case sw::Event::MouseMoved:
					//std::cout << "mouse moved " << event.mouseMove.x << " : " << event.mouseMove.y << "\n";
					break;
				case sw::Event::MouseButtonPressed:
					std::cout << "mouse pressed " << event.mouseClick.x << " : " << event.mouseClick.y << "\n";
					break;
				case sw::Event::MouseWheelScrolled:
					std::cout << "wheel " << event.mouseScroll.z << "\n";
					break;
				case sw::Event::MouseEntereOrLeft:
					std::cout << "left | entered\n";
					break;
				case sw::Event::Paint:
					// block::win32api
					BeginPaint(window.handle, &ps);
					FillRect(ps.hdc, &rect, brush);

					EndPaint(window.handle, &ps);
					// endblock::win32api
					break;
				case sw::Event::ButtonClick:
					if (buttonIsClicked(btn_clear, event))
					{
						widgetSetText(text_field, empty);
					}
					if (buttonIsClicked(btn_save, event))
					{
						if (GetSaveFileNameA(&ofn)) // win32api func
						{
							FILE* file_out = fopen(&filename[0], "w");	// Path shouldn't has russian letters
							fprintf(file_out, &widgetGetText(text_field, 1000)[0]);
							fclose(file_out);
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}