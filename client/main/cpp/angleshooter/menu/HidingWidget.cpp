#include "PreCompiledClient.h"
#include "HidingWidget.h"

HidingWidget::HidingWidget(std::function<bool()> predicate, sf::Vector2f position, sf::Vector2f size,const Identifier& texture) : MenuWidget(position, size, texture), predicate(std::move(predicate)) {}

HidingWidget::HidingWidget(std::function<bool()> predicate, sf::Vector2f position, float size, const Identifier& texture) : MenuWidget(position, size, texture), predicate(std::move(predicate)) {}

void HidingWidget::tick() {
	MenuWidget::tick();
	if (this->predicate()) {
		this->setOrigin({0, 0});
	} else {
		this->setOrigin({5000, 5000});
	}
}
