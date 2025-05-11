﻿#include "PreCompiledClient.h"
#include "OptionsManager.h"

namespace {
	auto fileName("run/options.json");
}

void OptionsManager::saveToFile() {
	nlohmann::json json;
	json["name"] = name;
	json["ip"] = ip;
	json["colour_r"] = r;
	json["colour_g"] = g;
	json["colour_b"] = b;
	json["masterVolume"] = masterVolume;
	json["musicVolume"] = musicVolume;
	json["soundVolume"] = soundVolume;
	json["onboarded"] = onboarded;
	json["fps"] = framesPerSecond;
	for (const auto& keybinding : InputManager::get().getKeybindings()) {
		json["keybindings"][keybinding->getId()->toString()] = keybinding->getKey();
	}
	std::filesystem::create_directories("run");
	if (std::ofstream file(fileName); file.is_open()) {
		file << json.dump(4);
		file.close();
		Logger::info("Saved options to file");
	} else {
		Logger::errorOnce("Failed to save options to file");
	}
}

void OptionsManager::loadFromFile() {
	if (std::ifstream file(fileName); file.is_open()) {
		Logger::info("Loading options from file");
		nlohmann::json json;
		file >> json;
		file.close();
		try {
			name = json.value("name", "Player");
			ip = json.value("ip", "127.0.0.1");
			r = json.value("colour_r", 0xFF);
			g = json.value("colour_g", 0xAA);
			b = json.value("colour_b", 0xAA);
			masterVolume = json.value("masterVolume", 100.);
			musicVolume = json.value("musicVolume", 0.);
			soundVolume = json.value("soundVolume", 0.);
			onboarded = json.value("onboarded", false);
			framesPerSecond = json.value("fps", 144);
			timePerFrame = 1. / framesPerSecond;
			for (const auto& keybinding : InputManager::get().getKeybindings()) {
				keybinding->loadBinding(json.value("keybindings", nlohmann::json()).value(keybinding->getId()->toString(), keybinding->getDefaultKey()));
			}
		} catch (const nlohmann::json::exception& e) {
			Logger::error("Failed to load options from file: " + std::string(e.what()));
			saveToFile();
		}
		Logger::info("Options loaded");
	} else {
		saveToFile();
		Logger::info("Creating new options file");
	}
}

OptionsManager::OptionsManager() {
	loadFromFile();
}

std::string OptionsManager::getName() const {
	return this->name;
}

std::string OptionsManager::getIp() const {
	return this->ip;
}

sf::Color OptionsManager::getColour() const {
	return sf::Color(r, g, b, 255);
}

double OptionsManager::getMasterVolume() const {
	return this->masterVolume;
}

double OptionsManager::getMusicVolume() const {
	return this->musicVolume;
}

double OptionsManager::getSoundVolume() const {
	return this->soundVolume;
}

bool OptionsManager::isOnboarded() const {
	return this->onboarded;
}

int OptionsManager::getFps() {
	return this->framesPerSecond;
}

double OptionsManager::getTimePerFrame() {
	return this->timePerFrame;
}

bool OptionsManager::isDebugEnabled() const {
	return this->debug;
}

void OptionsManager::setName(const std::string& name) {
	this->name = name;
	saveToFile();
	if (AngleShooterClient::get().server != nullptr) {
		auto packet = NetworkProtocol::C2S_UPDATE_NAME->getPacket();
		packet << name;
		AngleShooterClient::get().send(packet);
	}
}

void OptionsManager::setIp(const std::string& ip) {
	this->ip = ip;
	saveToFile();
}

void OptionsManager::setColour(sf::Color colour) {
	this->r = colour.r;
	this->g = colour.g;
	this->b = colour.b;
	saveToFile();
	if (AngleShooterClient::get().server != nullptr) {
		auto packet = NetworkProtocol::C2S_UPDATE_COLOUR->getPacket();
		packet << colour.r << colour.g << colour.b;
		AngleShooterClient::get().send(packet);
	}
}

void OptionsManager::setFps(int fps) {
	this->framesPerSecond = fps;
	this->timePerFrame = 1. / fps;
	saveToFile();
}

void OptionsManager::setMasterVolume(double volume) {
	this->masterVolume = volume;
	saveToFile();
}

void OptionsManager::setMusicVolume(double volume) {
	this->musicVolume = volume;
	saveToFile();
}

void OptionsManager::setSoundVolume(double volume) {
	this->soundVolume = volume;
	saveToFile();
}

void OptionsManager::setOnboarded(bool onboarded) {
	this->onboarded = onboarded;
	saveToFile();
}

void OptionsManager::setDebugEnabled(bool enabled) {
	this->debug = enabled;
}