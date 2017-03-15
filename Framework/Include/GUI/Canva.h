#pragma once

#include "Widget.h"

class Canva : public Widget
{
  public:
	Canva(int w, int h);
	void update(CommandStatus& status);
	void render(sf::RenderWindow& window);
};
