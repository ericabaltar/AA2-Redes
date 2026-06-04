#pragma once
#include "Scene.h"
#include "Button.h"
#include "NetworkManager.h"
#include "LoginStyle.h"
#include "InputField.h"

class LoginScene : public Scene
{
	sf::Font* font;

public:
	void Enter(SharedMemory* _sharedMemory) override {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);

		//Nickname
		sf::RectangleShape nicknameRect;
		nicknameRect.setSize(nicknameRectSize);
		nicknameRect.setPosition({ WINDOW_WIDTH * nicknameRectAnchorX - nicknameRect.getSize().x / 2, WINDOW_HEIGHT * nicknameRectAnchorY });
		nicknameRect.setFillColor(nicknameRectColour);
		InputField* nicknameField = new InputField(nicknameRect, sf::Text(*font), nicknameMaxChars);

		objects.push_back(nicknameField);

		//Password
		sf::RectangleShape passwordRect;
		passwordRect.setSize(passwordRectSize);
		passwordRect.setPosition({ WINDOW_WIDTH * passwordRectAnchorX - passwordRect.getSize().x / 2, WINDOW_HEIGHT * passwordRectAnchorY });
		passwordRect.setFillColor(passwordRectColour);
		InputField* passwordField = new InputField(passwordRect, sf::Text(*font), passwordMaxChars);
		passwordField->isCensored = true;

		objects.push_back(passwordField);

		//Login
		sf::RectangleShape loginRect;
		loginRect.setSize(loginRectSize);
		loginRect.setPosition({ WINDOW_WIDTH * loginRectAnchorX - loginRect.getSize().x / 2, WINDOW_HEIGHT * loginRectAnchorY });
		loginRect.setFillColor(loginRectColour);
		Button* loginButton = new Button(loginRect, sf::Text(*font, loginButtonLabel), [nicknameField, passwordField]() {
			NT->SendLoginAttemptServerPacket(nicknameField->realStr, passwordField->realStr);
			});

		objects.push_back(loginButton);

		//Register
		sf::RectangleShape registerRect;
		registerRect.setSize(registerRectSize);
		registerRect.setPosition({ WINDOW_WIDTH * registerRectAnchorX - registerRect.getSize().x / 2, WINDOW_HEIGHT * registerRectAnchorY });
		registerRect.setFillColor(registerRectColour);
		Button* registerButton = new Button(registerRect, sf::Text(*font, registerButtonLabel), [nicknameField, passwordField]() {
			NT->SendRegisterAttemptServerPacket(nicknameField->realStr, passwordField->realStr);
			});

		objects.push_back(registerButton);
	}

	bool Update(sf::RenderWindow& window, float dt) override
	{
		if (NT->GetSuccessfulLogin())
			nextScene = SceneOption::LOBBY;

		return Scene::Update(window, dt);
	}
};

