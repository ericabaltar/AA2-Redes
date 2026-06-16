#pragma once 
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "AnimatedSprite.h" 
#include "Object.h"

class Character : public Object {
protected:
	sf::SoundBuffer quackBuffer;
	sf::Sound quackSound;

	const float scale = 4.f;
	float width;
	float height;

	AnimatedSprite sprite;
	float walkAnimVelocityThreshold = 20.f;

	Vector2 velocity;

	const float jumpBufferTime = 0.12f;
	float jumpBufferTimer = 0.f;

	bool isOnGround = false;
	bool facingRight = true;

	bool isQuacking = false;
	bool isShooting = false;

	Vector2 colliderOffset;

	const int lives = 3;
	const int healthPoints = 5;
	int currentLives = lives;
	int currentHealthPoints = healthPoints;

public:
	Character(const std::string& texturePath = "assets/grey_duck.png");

protected:
	void HandleAnimation(float dt);

public:
	void Update(float dt) override;

	virtual void Quack();
	virtual void Shoot(bool towardsRight);
	virtual void Shoot();

	void Render(sf::RenderWindow& window) override { window.draw(sprite.GetSprite()); }

	virtual void OnCollisionEnter(Object* other, const CollisionInfo& collisionInfo) override;

	inline bool IsDead() { return currentLives <= 0; }

	inline Vector2 GetPosition() { return transform->position; }
	inline void SetPosition(const Vector2 newPosition) { transform->position = newPosition; }
	void SetInterpolatedPosition(Vector2 newPosition);
	void HandleEvent(const sf::Event& event) override {}

	// Añadido para que el GameScene pueda aplicar la vida validada por el servidor
	void SetHealth(int health, int livesUpdated) {
		currentHealthPoints = health;
		currentLives = livesUpdated;
	}
};