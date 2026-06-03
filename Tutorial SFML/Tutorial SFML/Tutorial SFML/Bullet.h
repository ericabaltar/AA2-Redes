#pragma once
#include "Object.h"
#include <iostream>

class Bullet : public Object
{
private:
	const float speed = 700.f;
	
	sf::Texture texture;
	sf::Sprite sprite;

	const float scale = 4.f;
	float width;
	float height;

	Vector2 velocity;

	float lifeTime = 5.f;

	Object* owner = nullptr;

public:
	Bullet(const std::string& texturePath = "assets/bullet.png")
		: texture(texturePath), sprite(texture)
	{
		transform->position = { 0.f, 500.f };
		sprite.setScale({ scale, scale });

		velocity = Vector2(0, 0);
		width = sprite.getLocalBounds().size.x * scale;
		height = sprite.getLocalBounds().size.y * scale;

		collider->SetSize(Vector2(width, height));
	}

	void Init(Object* ownerObj, Vector2 pos, bool facingRight)
	{
		owner = ownerObj;

		transform->position = pos;

		float signedSpeed = facingRight ? speed : -speed;
		velocity = Vector2(signedSpeed, 0);
		
		if (!facingRight)
		{
			sprite.setScale({ -scale, scale });
		}
	}

	void Update(float dt) override
	{
		transform->position = transform->position + velocity * dt;
		sprite.setPosition(sf::Vector2f(transform->position.x, transform->position.y));
	
		lifeTime -= dt;

		if (lifeTime <= 0.f)
			Destroy();

		collider->SetTopLeft(transform->position - Vector2(width / 2.f, height / 2.f));
	}

	void Render(sf::RenderWindow& window) override
	{
		window.draw(sprite);
	}

	void OnCollisionEnter(Object* other)
	{
		if (other == owner)
			return;

		Destroy();
	}
};

