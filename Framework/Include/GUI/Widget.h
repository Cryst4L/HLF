////////////////////////////////////////////////////////////////////////////////
// Base class of the GUI basic elements such as: buttons, sliders , ...
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Style.h"
#include "Commands.h"

class Widget
{
protected :

	sf::Vector2f m_position;
	sf::Vector2f m_dimension;

public :

	// State Updating Method
    virtual void update(CommandStatus& status) = 0;

	// Rendering Method
	virtual	void render(sf::RenderWindow& window) = 0;
	
	// Convenience Methods
	void setPosition(int x, int y);
	void setDimension(int w, int h);

	virtual ~Widget() {};
};