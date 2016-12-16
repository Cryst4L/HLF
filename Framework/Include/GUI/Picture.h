/////////////////////////////////////////////////////////////////////////////////////
// Convenience widget unifying SFML image, texture and sprites management
// for displaying pictures.
/////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Widget.h"

class Picture : public Widget
{
    private:
	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

    public:
	Picture();
	Picture(sf::Image& image);

	void update(CommandStatus& status);
	void render(sf::RenderWindow& window);

	void setScale(double scale);

	void loadFromFile(const std::string &filename);
	void setImage(sf::Image& image);
};
