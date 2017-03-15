#include "GUI/Canva.h"

Canva::Canva(int w, int h)
{
	m_dimension = sf::Vector2f(w, h);
}

void Canva::update(CommandStatus& status) {}

void Canva::render(sf::RenderWindow& window)
{
	sf::RectangleShape shape(m_dimension);

	shape.setPosition(m_position);
	shape.setFillColor(COLOR_FILL);

	shape.setOutlineColor(COLOR_BASE);
	shape.setOutlineThickness(SHAPE_OUTLINE);

	window.draw(shape);
}
