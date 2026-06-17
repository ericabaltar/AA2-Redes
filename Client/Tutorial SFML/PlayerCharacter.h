#pragma once 
#include "Character.h"
#include "MovementPacket.h"
#include <vector>

class PlayerCharacter : public Character {
private:
	float inputX = 0;
	bool isFocused = true;

	const float acceleration = 1800.f;
	const float maxSpeed = 1500.f;
	const float friction = 8.f;

	const float gravity = 2000.f;
	const float jumpForce = -1050.f;

	const float coyoteTime = 0.12f;
	float coyoteTimer = 0.f;

	void HandleInput(float dt);

	void ApplyPhysics(float dt);

	int movementPacketIndex = 0;
	MovementPacket GenerateMovementPacket();
	std::vector<MovementPacket> movementPackets;

public:
	PlayerCharacter() : Character("assets/white_duck.png") {
		transform->position = { 100.f, 300.f };
	}

	void HandleEvent(const sf::Event& event) override;
	void Update(float dt) override;

	void Quack() override;
	void Shoot() override;

	inline void ResetJumpBuffer() { jumpBufferTimer = jumpBufferTime; }

	void OnCollisionEnter(Object* other, const CollisionInfo& collisionInfo) override;
};