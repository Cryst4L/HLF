#pragma once

#include "Widget.h"

class Canva : public Widget
{
  private:
	sf::Color m_fill_color;

  public:
	Canva(int w, int h);
	void update(CommandStatus& status);
	void render(sf::RenderWindow& window);
	void setFillColor(sf::Color color);
};
