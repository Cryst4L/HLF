#pragma once

#include "InitGUI.h"
#include <SFML/Graphics.hpp>

// Fontes //////////////////////////////////////////////////////////////////////
static const char* FONT_NAME = "DroidSansMono.ttf";

enum FontSize {
	FONT_SMALL	= 13,
	FONT_MEDIUM = 17,
	FONT_LARGE	= 25
};

// Colors //////////////////////////////////////////////////////////////////////

const sf::Color COLOR_BASE	= sf::Color(  0,   0,   0);
const sf::Color COLOR_FILL	= sf::Color(220, 220, 220);
const sf::Color COLOR_WALL	= sf::Color(180, 180, 180);
const sf::Color COLOR_BUT0	= sf::Color( 92,  92,  92);
const sf::Color COLOR_BUT1	= sf::Color(128, 128, 128);
const sf::Color COLOR_LOCK	= sf::Color( 64, 128, 255);
/*
const sf::Color COLOR_BASE	= sf::Color(255, 255, 255);
const sf::Color COLOR_FILL	= sf::Color(  0,   0,   0);
const sf::Color COLOR_WALL	= sf::Color( 0,   50,  75);
const sf::Color COLOR_BUT0	= sf::Color( 64,  64,  64);
const sf::Color COLOR_BUT1	= sf::Color(128, 128, 128);
const sf::Color COLOR_LOCK	= sf::Color(255, 255,  64);
*/
// Misc. ///////////////////////////////////////////////////////////////////////

const int 	ANTI_ALIASING	= 2;
const float SHAPE_OUTLINE	= 1.5f;
