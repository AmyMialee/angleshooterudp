#include "PreCompiledClient.h"
#include "FloatingWidget.h"

FloatingWidget::FloatingWidget(sf::Vector2f first, sf::Vector2f second, sf::Vector2f size, const Identifier& texture, int cycleSpeed) : MenuWidget(first, size, texture) {
	this->first = first;
	this->second = second;
	this->cycleSpeed = cycleSpeed;
}

FloatingWidget::FloatingWidget(sf::Vector2f first, sf::Vector2f second, float size, const Identifier& texture, int cycleSpeed) : MenuWidget(first, size, texture) {
	this->first = first;
	this->second = second;
	this->cycleSpeed = cycleSpeed;
}

void FloatingWidget::tick() {
	MenuWidget::tick();
	if (cycleSpeed > 0) {
		cycle++;
		if (cycle > cycleSpeed) cycle = 0;
		const auto delta = sin((float)cycle / (float)cycleSpeed * 2.f * M_PI) * 0.5f + 0.5f;
		setPosition(Util::lerp(delta, first, second));
	}
}