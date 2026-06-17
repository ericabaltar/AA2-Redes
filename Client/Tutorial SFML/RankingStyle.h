#pragma once
#include <SFML/Graphics.hpp>
#include "config.h"

const sf::Font emptyFont = sf::Font::Font();

const int rankingListLength = 10;
const float rankingsWidth = WINDOW_WIDTH * 0.4f;
const float rankingsHeight = WINDOW_HEIGHT * 0.1f;

const float rankingsAnchorX = 0.05f;
const float rankingsAnchorY = 0.05f;

const float rankingsSeparation = WINDOW_HEIGHT * 0.025f;

const sf::Color rankingsBackground = sf::Color::Cyan;
const int rankingsCharacterSize = 40;
const sf::Text::Style rankingsTextStyle = sf::Text::Style::Bold;
