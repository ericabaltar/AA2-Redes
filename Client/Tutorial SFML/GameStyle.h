#pragma once
#include "config.h"
#include <SFML/Graphics.hpp>

const sf::Font nullFont = sf::Font::Font();

#pragma region Board

const sf::Color colours[PLAYER_COUNT + 1] = {
		sf::Color::Black, //Default
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Yellow
};

const float boardSideLength = WINDOW_WIDTH * 0.35f;

const float boardAnchorX = 0.3f;
const float boardAnchorY = 0.2f;

const float cellPadding = 0.1f;

#pragma endregion

#pragma region Players

const float playersWidth = WINDOW_WIDTH * 0.2f;
const float playersHeight = WINDOW_HEIGHT * 0.1f;

const float playersAnchorX = 0.05f;
const float playersAnchorY = 0.2f;

const float playersSeparation = WINDOW_HEIGHT * 0.05f;

const sf::Color playersBackground = sf::Color::Cyan;
const sf::Color activePlayersBackground = sf::Color::Yellow;
const int playersCharacterSize = 40;
const sf::Text::Style playersTextStyle = sf::Text::Style::Bold;

#pragma endregion
