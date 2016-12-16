//////////////////////////////////////////////////////////////////////
// Everything a widget needs to know a about the user inputs
// is encapsulated in the 'CommandStatus' structure.
//////////////////////////////////////////////////////////////////////
#pragma once

#include <SFML/System.hpp>

struct MouseStatus {
	int x, y;
	bool pressed;
	bool clicked;
};

struct KeyboardStatus {
	int code;
	int unicode;
	bool stroked;
};

typedef struct CommandStatus {
	MouseStatus mouse;
	KeyboardStatus keyboard;
} CommandStatus;
