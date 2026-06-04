#pragma once
#include "Transform.h"
#include "AABB.h"
#include <vector>

class Rigidbody
{
private:
	std::vector<AABB*> colliders;
	Transform* transform;
	Vector2 velocity;
	Vector2 acceleration;
	float linearDrag;

	Vector2 gravity = Vector2(0.f, 980.f);
	bool useGravity = false;

public:
	Rigidbody(Transform* _transform, bool _useGravity = false)
		: transform(_transform),
		velocity(Vector2()),
		acceleration(Vector2()),
		linearDrag(0.0f),
		useGravity(_useGravity) { };
	~Rigidbody()
	{
		for (AABB* collider : colliders)
			delete collider;
	};
	void AddCollider(AABB* collider) { colliders.push_back(collider); }
	bool CheckCollision(const Rigidbody* other)
	{
		for (AABB* myCol : colliders)
		{
			for (AABB* otherCol : other->colliders)
			{
				if (myCol->CheckOverlappingAABB(otherCol))
					return true;
			}
		}
		return false;
	};
	bool CheckOverlappingPoint(Vector2 point)
	{
		for (AABB* myCol : colliders)
		{
			if (myCol->CheckOverlappingPoint(point))
				return true;
		}
		return false;
	};
	void Update(float dt)
	{
		if (useGravity)
			AddForce(gravity);
		
		// 1 -> Update the velocity
		velocity = velocity + acceleration * dt;

		// 2 -> Update the DRAG
		velocity = velocity * (1.0f / (1.0f + dt));

		// 3 -> Update position
		transform->position = transform->position + velocity * dt;

		// 4 -> Reset acceleration
		acceleration = Vector2();

		// 5 -> Update collider positioning
		Vector2 offset = ((Vector2(-transform->size.x, -transform->size.y)) / 2.0f) * transform->scale;


		for (AABB* col : colliders)
		{
			col->SetTopLeft(transform->position + offset);
			col->SetSize(transform->size * transform->scale);
		}

	}
	inline void AddForce(Vector2 force) { acceleration = acceleration + force; }
	inline Vector2 const GetVelocity() { return velocity; }
	inline void SetVelocity(Vector2 velocity) { this->velocity = velocity; }
	inline void SetLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }
};
