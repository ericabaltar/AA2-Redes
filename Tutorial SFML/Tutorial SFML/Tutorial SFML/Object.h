#pragma once
#include "Transform.h"
#include "Rigidbody.h"
#include <SFML/Graphics.hpp>

class Object
{
private:
	bool isPendingDestroy = false;

protected:
	Transform* transform;
	AABB* collider;

public:
	Object()
	{
		transform = new Transform();
		collider = new AABB(Vector2(0.f,0.f), Vector2(0.f, 0.f));
	}

	~Object()
	{
		delete transform;
		delete collider;
	}

	virtual void Update(float dt) {}
	virtual void Render(sf::RenderWindow& window) {}
	virtual void HandleEvent(const sf::Event& event) {}

	bool CheckCollision(const AABB* other)
	{
		if (collider->CheckOverlappingAABB(other))
			return true;

		return false;
	};
	virtual void OnCollisionEnter(Object* other, const CollisionInfo& collisionInfo) {}

	inline bool IsPendingDestroy() const { return isPendingDestroy; }
	inline void Destroy() { isPendingDestroy = true; }

	inline Transform* const GetTransform() { return transform; }
	void SetTransform(Transform* t) { transform = t; };
	void SetPosition(float x, float y) { transform->position = Vector2(x, y); };
	inline AABB* const GetCollider() { return collider; }
};

