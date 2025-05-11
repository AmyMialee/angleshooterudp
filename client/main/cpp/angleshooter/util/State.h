#pragma once

class StateManager;

class State : public sf::Drawable {
public:
	typedef std::unique_ptr<State> Pointer;

	State() = default;
	~State() override = default;
	State(const State&) = delete;
	State& operator=(const State&) = delete;
	virtual void init() = 0;
	virtual void destroy();
	/**
	 * @return Should continue ticking States
	 */
	virtual bool tick() = 0;
	/**
	 * @return Should continue passing events down to States
	 */
	virtual bool handleEvent(const sf::Event& event) = 0;
	virtual Identifier getStateId() = 0;

protected:
	void requestStackPush(const Identifier& id);
	void requestStackPop();
	void requestStackClear();
};