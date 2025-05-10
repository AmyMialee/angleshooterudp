﻿#include "PreCompiledClient.h"
#include "StateManager.h"

#include "../states/ServerListState.h"

StateManager::PendingChange::PendingChange(StackMove action, Identifier id) : action(action), id(std::move(id)) {}

State::Pointer StateManager::create(const Identifier& id) {
	const auto found = stateMap.find(id);
	if (found == stateMap.end()) {
		Logger::error("State " + id.getSpace() + " " + id.getPath() + " not found!");
		return nullptr;
	}
	auto state = found->second();
	return state;
}

void StateManager::applyChanges() {
	for (const auto& change : pending) {
		switch (change.action) {
			case StackMove::PUSH:
				stack.emplace_back(create(change.id), false);
				continue;
			case StackMove::POP:
				stack.back().first->destroy();
				stack.pop_back();
				continue;
			case StackMove::CLEAR:
				for (auto& [state, initialized] : std::ranges::reverse_view(stack)) state->destroy();
				stack.clear();
		}
	}
	if (!pending.empty()) {
		std::stringstream stream;
		stream << "State Change, Stack: ";
		auto first = true;
		for (const auto& [state, initialized] : std::ranges::reverse_view(stack)) {
			if (!first) stream << " <- ";
			stream << state->getStateId().toString();
			first = false;
		}
		Logger::debug(stream.str());
	}
	pending.clear();
	if (this->stack.empty()) {
		AngleShooterClient::get().window.close();
	} else if (!stack.back().second) {
		stack.back().second = true;
		stack.back().first->init();
	}
}

StateManager::StateManager() {
	registerState<SplashState>(SplashState::getId());
	registerState<MenuState>(MenuState::getId());
	registerState<SettingsState>(SettingsState::getId());
	registerState<OnboardingState>(OnboardingState::getId());
	registerState<GameState>(GameState::getId());
	registerState<PauseState>(PauseState::getId());
	registerState<ServerListState>(ServerListState::getId());
}

void StateManager::loadAssets() {
	for (const auto& second : stateMap | std::views::values) second()->loadAssets();
}

void StateManager::tick() {
	for (const auto& [state, initialized] : std::ranges::reverse_view(stack)) {
		if (!state->tick()) break;
	}
	applyChanges();
}

void StateManager::render(float deltaTime) {
	if (stack.empty()) return;
	auto iterator = stack.end() - 1;
	for (; iterator != stack.begin(); --iterator) if (!iterator->first->shouldRenderNextState()) break;
	for (; iterator != stack.end(); ++iterator) iterator->first->render(deltaTime);
}

void StateManager::handleEvent(const sf::Event& event) {
	for (const auto& [state, initialized] : std::ranges::reverse_view(stack)) if (!state->handleEvent(event)) break;
	applyChanges();
}

Identifier StateManager::getStateId() const {
	return stack.empty() ? Identifier::empty : stack.back().first->getStateId();
}

void StateManager::push(const Identifier& id) {
	pending.emplace_back(StackMove::PUSH, id);
}

void StateManager::pop() {
	pending.emplace_back(StackMove::POP);
}

void StateManager::clear() {
	pending.emplace_back(StackMove::CLEAR);
}

bool StateManager::isEmpty() const {
	return stack.empty();
}

State::Pointer* StateManager::getCurrentState() {
	return stack.empty() ? nullptr : &stack.back().first;
}
