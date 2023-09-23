Пример использования библиотеки на классах C++.
Совмещенный с использованием стандартного win32api

	#include <SWL.h>

	#include <Commdlg.h>
	#include <fileapi.h>

	int main()
	{
		sw::Vector2u win_size(900, 500);
		sw::Vector2i win_locate(600, 300);
		std::string win_title = "My title";
		sw::Font font = sw::Font(20, 400, "Courier New");

		sw::Window window = sw::Window(win_locate, win_size, win_title);
		
		int click_count = 0;
		sw::Widget btn_clear = sw::Widget(font, sw::WidgetType::Button, sw::Vector2u(200, 50), sw::Vector2u(win_size.x - 400, win_size.y - 180), "clear");
		window.add(btn_clear);

		sw::Widget btn_save = sw::Widget(font, sw::WidgetType::Button, sw::Vector2u(200, 50), sw::Vector2u(200, win_size.y - 180), "save");
		window.add(btn_save);

		sw::Widget label = sw::Widget(sw::Font(sw::TextAlign::Center, 35, 700, false, false, false, "Arial"), sw::WidgetType::Label, sw::Vector2u(win_size.x, 45), sw::Vector2u(0, 20), "It's my Blacknote, on SWM");
		window.add(label);

		sw::Widget text_field = sw::Widget(font, sw::WidgetType::TextField, sw::Vector2u(500, 200), sw::Vector2u(win_size.x / 2 - 250, 80), "Redact me");
		window.add(text_field);

		// BLOCK::WINAPI
		char filename[256] = "myNote.txt";
		OPENFILENAMEA ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = window.getHandle();
		ofn.lpstrFile = filename;
		ofn.nMaxFile = sizeof(filename);
		ofn.lpstrFilter = ".txt";
		ofn.lpstrTitle = NULL;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		// ENDBLOCK::WIN32API

		while (window.isOpen())
		{
			sw::Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sw::Event::Closed:
					window.close();
					break;
				case sw::Event::KeyPressed:
					std::cout << char(event.key.code)<< "\n";
					break;
				case sw::Event::MouseMoved:
					//std::cout << "mouse move " << event.mouseMove.x << " : " << event.mouseMove.y << "\n";
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
				case sw::Event::ButtonClick:
					if (btn_clear.isClicked(event))
					{
						text_field.setText("");
					}
					if (btn_save.isClicked(event))
					{
						if (GetSaveFileNameA(&ofn)) // win32api func
						{
							FILE* fileOut = fopen(&filename[0], "w");	// Path shouldn't have russian letters
							fprintf(fileOut, &text_field.getText(1000)[0]);
							fclose(fileOut);
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}