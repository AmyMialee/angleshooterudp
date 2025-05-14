#pragma once

enum class MenuInput : std::uint8_t {
	PRESS,
	ESCAPE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RELEASE_LEFT,
	RELEASE_RIGHT,
};

inline const char* toString(MenuInput move) {
	switch (move) {
		case MenuInput::PRESS: return "PRESS";
		case MenuInput::ESCAPE: return "ESCAPE";
		case MenuInput::UP: return "UP";
		case MenuInput::DOWN: return "DOWN";
		case MenuInput::LEFT: return "LEFT";
		case MenuInput::RIGHT: return "RIGHT";
		case MenuInput::RELEASE_LEFT: return "RELEASE_LEFT";
		case MenuInput::RELEASE_RIGHT: return "RELEASE_RIGHT";
	}
	Logger::errorOnce("Invalid MenuInput used for toString");
	return "UNKNOWN";
}