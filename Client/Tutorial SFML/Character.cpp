#include "Character.h" 
#include "Bullet.h" 
#include "Ground.h" 
#include "Spawner.h"

void Character::HandleAnimation(float dt) {
	if (isQuacking || isShooting) {
		sprite.Update(dt);

		if (sprite.IsCurrentAnimationFinished())
		{
			isQuacking = false;
			isShooting = false;
			sprite.StartAnimation("idle", true);
		}
		return;
	}

	bool isMoving = std::abs(velocity.x) >= walkAnimVelocityThreshold;

	if (isMoving)
		sprite.StartAnimation("walk");
	else
		sprite.StartAnimation("idle");

	sprite.FlipHorizontally(!facingRight);

	sprite.Update(dt);
}

Character::Character(const std::string& texturePath) : sprite(texturePath), quackSound(quackBuffer) {
	transform->position = { 400.f, 300.f };
	sprite.SetScale({ scale, scale });

	sprite.AddAnimation("idle", Animation(32, 32, 0, 1, 0));
	sprite.AddAnimation("walk", Animation(32, 32, 1, 6, 0.1f));
	sprite.AddAnimation("shoot", Animation(32, 32, 2, 5, 0.05f, false));
	sprite.AddAnimation("quack", Animation(32, 32, 3, 1, 0.5f, false));

	sprite.StartAnimation("idle");

	velocity = Vector2(0, 0);

	width = sprite.GetWidth() * scale;
	height = sprite.GetHeight() * scale;

	float colliderWidth = width * 0.5f;
	float colliderHeight = height * (2.f / 3.f);

	collider->SetSize(Vector2(colliderWidth, colliderHeight));

	float offsetY = (height / 2.f) - (colliderHeight / 2.f);
	colliderOffset = -Vector2(colliderWidth / 2.f, colliderHeight / 2.f) + Vector2(0.f, offsetY);

	collider->SetTopLeft(transform->position + colliderOffset);

	if (!quackBuffer.loadFromFile("assets/quack.wav"))
		std::cout << "Error cargando quack.wav" << std::endl;

	quackSound.setVolume(20.f);
}

void Character::Update(float dt) {
	HandleAnimation(dt);
	collider->SetTopLeft(transform->position + colliderOffset);
	sprite.SetPosition(transform->position);
}

void Character::Quack() {
	isQuacking = true;
	sprite.StartAnimation("quack");
	quackSound.play();
}

void Character::Shoot(bool towardsRight)
{
	facingRight = towardsRight;
	sprite.FlipHorizontally(!facingRight);

	isShooting = true;
	sprite.StartAnimation("shoot");

	Bullet* bullet = new Bullet();

	float offset = towardsRight ? 40.f : -40.f;
	Vector2 position = Vector2(transform->position.x + offset, transform->position.y);

	bullet->Init(this, position, towardsRight);

	SPAWN.SpawnObject(bullet);
}

void Character::Shoot()
{
	Shoot(facingRight);
}

void Character::OnCollisionEnter(Object* other, const CollisionInfo& collisionInfo) {
	if (dynamic_cast<Ground*>(other)) {
		if (collisionInfo.penetration < 0.01f) return;

		transform->position.x = transform->position.x + collisionInfo.normal.x * collisionInfo.penetration;
		transform->position.y = transform->position.y + collisionInfo.normal.y * collisionInfo.penetration;

		if (collisionInfo.normal.y != 0)
			velocity.y = 0;

		if (collisionInfo.normal.x != 0)
			velocity.x = 0;
	}
}

void Character::SetInterpolatedPosition(const Vector2 newPosition) {
	if (newPosition.x > transform->position.x || newPosition.x < transform->position.x)
	{
		velocity.x = walkAnimVelocityThreshold;
	}
	else {
		velocity.x = 0.f;
	}
	transform->position = newPosition;
}