#pragma once
#include <SFML/Graphics.hpp>
#include "config.h"

const sf::Vector2f nicknameRectSize = { WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.15f };
const float nicknameRectAnchorX = 0.5f;
const float nicknameRectAnchorY = 0.1f;
const sf::Color nicknameRectColour = sf::Color::Red;
const int nicknameMaxChars = 10;

const sf::Vector2f passwordRectSize = { WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.15f };
const float passwordRectAnchorX = 0.5f;
const float passwordRectAnchorY = 0.3f;
const sf::Color passwordRectColour = sf::Color::Red;
const int passwordMaxChars = 10;

const sf::Vector2f loginRectSize = { WINDOW_WIDTH * 0.2f, WINDOW_HEIGHT * 0.15f };
const float loginRectAnchorX = 0.5f;
const float loginRectAnchorY = 0.5f;
const sf::Color loginRectColour = sf::Color::Red;
const std::string loginButtonLabel = "Login";

const sf::Vector2f registerRectSize = { WINDOW_WIDTH * 0.2f, WINDOW_HEIGHT * 0.15f };
const float registerRectAnchorX = 0.5f;
const float registerRectAnchorY = 0.7f;
const sf::Color registerRectColour = sf::Color::Red;
const std::string registerButtonLabel = "Register";