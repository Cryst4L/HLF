#include "GUI/Canva.h"

Canva::Canva(int w, int h)
{
	m_dimension = sf::Vector2f(w, h);
	m_fill_color = COLOR_BASE;
}

void Canva::update(CommandStatus& status) {}

void Canva::render(sf::RenderWindow& window)
{
	sf::RectangleShape shape(m_dimension);

	shape.setPosition(m_position);
	shape.setFillColor(m_fill_color);

	shape.setOutlineColor(COLOR_MAIN);
	shape.setOutlineThickness(SHAPE_OUTLINE);

	window.draw(shape);
}

void Canva::setFillColor(sf::Color color) {
	m_fill_color = color;
}