#include "GUI/InitGUI.h"

sf::Mutex globalMutex;

bool XINIT = XInitThreads();

sf::Font initFont(const std::string name)
{
	sf::Font font;
	if (!font.loadFromFile("../Framework/Assets/" + name)) {
		exit(1);
	}
	return font;
}
