#pragma once

#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#undef Success

extern sf::Mutex globalMutex;

extern bool XINIT;

sf::Font initFont(const std::string name);

