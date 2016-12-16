#include "GUI/Picture.h"

Picture::Picture() {}

Picture::Picture(sf::Image& image) 
:	m_image(image) {
	m_dimension = sf::Vector2f(image.getSize().x, image.getSize().y);
}

void Picture::update(CommandStatus& status) {}

void Picture::render(sf::RenderWindow& window) {    
	m_texture.loadFromImage(m_image);
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	window.draw(m_sprite);
}

void Picture::setScale(double scale) {
	m_sprite.setScale(scale, scale);
}

void Picture::loadFromFile(const std::string &filename) {
	m_image.loadFromFile(filename);
}

void Picture::setImage(sf::Image& image) {
    m_image = image;
	m_dimension = sf::Vector2f(image.getSize().x, image.getSize().y);
}

