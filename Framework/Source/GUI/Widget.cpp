#include "GUI/Widget.h"

void Widget::setPosition(int x, int y) {
	m_position = sf::Vector2f(x, y);
}

void Widget::setDimension(int w, int h) {
	m_dimension = sf::Vector2f(w, h);
}
