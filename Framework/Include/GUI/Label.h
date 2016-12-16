#pragma once

#include "Widget.h"

class Label : public Widget
{
    private:

	sf::Text m_text;
	sf::Font m_font;

	FontSize m_font_size;
	sf::Vector2f m_offset;

    public:

	Label(std::string string);

	void update(CommandStatus& status);
	void render(sf::RenderWindow& window);

	void setText(std::string words);
};
