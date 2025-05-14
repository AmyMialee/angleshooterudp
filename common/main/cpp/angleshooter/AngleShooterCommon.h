#pragma once

class AngleShooterCommon {
public:
	static constexpr auto PORT = 62565;
	static constexpr auto TIME_PER_TICK = 1. / 60.;
	static constexpr auto TIMEOUT = 30.f;
	static constexpr auto MAX_BULLETS = 360;
	static constexpr auto BULLET_CHARGE = 4;
	static constexpr auto BULLET_DRAIN = 8;

};