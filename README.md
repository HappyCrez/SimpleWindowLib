# SimpleWindowLib
Пример использования библиотеки

#include <SWL.h>
int main() {
	sw::Vector2u winSize(400, 400);
	sw::Vector2u winLocate(600, 300);
	std::string winTitle = "My title";

	sw::Window window = sw::Window(winLocate, winSize, winTitle);

	while (window.isOpen()) {

		sw::Event event;
		while (window.pollEvent(event)) {

			switch (event.type) {
			case sw::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
	}
}
