Пример использования библиотеки на классах C++.
Совмещенный с использованием стандартного win32api

	#include <SWL.h>

	#include <Commdlg.h>
	#include <fileapi.h>

	int main()
	{

		sw::Vector2u winSize(900, 500);
		sw::Vector2u winLocate(600, 300);
		std::string winTitle = "My title";
		sw::Font font = sw::Font(20, 400, "Courier New");

		sw::Window window = sw::Window(winLocate, winSize, winTitle);
		
		int clickCount = 0;
		sw::Widget btnClear = sw::Widget(font, sw::WidgetType::Button, sw::Vector2u(200, 50), sw::Vector2u(winSize.x - 400, winSize.y - 180), "clear");
		window.add(btnClear);

		sw::Widget btnSave = sw::Widget(font, sw::WidgetType::Button, sw::Vector2u(200, 50), sw::Vector2u(200, winSize.y - 180), "save");
		window.add(btnSave);


		sw::Widget label = sw::Widget(sw::Font(sw::TextAlign::Center, 35, 700, false, false, false, "Arial"), sw::WidgetType::Label, sw::Vector2u(winSize.x, 45), sw::Vector2u(0, 20), "It's my Blacknote, on SWM");
		window.add(label);

		sw::Widget textField = sw::Widget(font, sw::WidgetType::TextField, sw::Vector2u(500, 200), sw::Vector2u(winSize.x / 2 - 250, 80), "Redact me");
		window.add(textField);

		// BLOCK::WINAPI
		char fileName[256] = "myNote.txt";
		OPENFILENAMEA ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = window.getHandle();
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = sizeof(fileName);
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
					break;
				case sw::Event::MouseButtonPressed:
					break;
				case sw::Event::ButtonClick:
					if (btnClear.isClicked(event))
					{
						textField.setText("");
					}
					if (btnSave.isClicked(event))
					{
						if (GetSaveFileNameA(&ofn)) // win32api func
						{
							FILE* fileOut = fopen(&fileName[0], "w");	// Path shouldn't have russian letters
							fprintf(fileOut, &textField.getText(1000)[0]);
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