#include "GUI/Picture.h"

Picture::Picture()
 	: m_show_outline(false) {}

Picture::Picture(sf::Image& image)
	:	m_image(image), m_show_outline(false)
{
	m_dimension = sf::Vector2f(image.getSize().x, image.getSize().y);
}

void Picture::update(CommandStatus& status) {}

void Picture::render(sf::RenderWindow& window)
{
	m_texture.loadFromImage(m_image);
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	window.draw(m_sprite);

	if (m_show_outline) {
		sf::FloatRect picture_bounds = m_sprite.getGlobalBounds();
		sf::Vector2f picture_size(picture_bounds.width, picture_bounds.height);
		sf::RectangleShape picture_outline(picture_size);
		picture_outline.setPosition(m_position);

		picture_outline.setOutlineColor(COLOR_MAIN);
		picture_outline.setFillColor(sf::Color::Transparent);
		picture_outline.setOutlineThickness(SHAPE_OUTLINE);
		window.draw(picture_outline);
	}

}

void Picture::setScale(double scale)
{
	m_sprite.setScale(scale, scale);
}

void Picture::loadFromFile(const std::string &filename)
{
	m_image.loadFromFile(filename);
}

void Picture::setImage(sf::Image& image)
{
	m_image = image;
	m_dimension = sf::Vector2f(image.getSize().x, image.getSize().y);
}

void Picture::showOutline(bool show)
{
	m_show_outline = show;
}
