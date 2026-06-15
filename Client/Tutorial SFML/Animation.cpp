#include "Animation.h"
#include <iostream>

Animation::Animation(int frameWidth, int frameHeight, int row, int columns, float frameTime, bool loop)
{
	this->frameTime = frameTime;
	this->loop = loop;

	for (int i = 0; i < columns; i++)
	{
		sf::IntRect frame;

		frame.position.x = i * frameWidth;
		frame.position.y = row * frameHeight;

		frame.size.x = frameWidth;
		frame.size.y = frameHeight;

		frames.push_back(frame);
	}
}

void Animation::Update(float dt)
{
	if (frames.empty()) return;

	timer += dt;

	if (timer >= frameTime)
	{
		timer = 0.f;

		if (loop)
		{
			currentFrame = (currentFrame + 1) % frames.size();
		}
		else
		{
			if (currentFrame < frames.size() - 1)
				currentFrame++;
			else
				finished = true;
		}
	}
}

void Animation::Reset()
{
	timer = 0.f;
	currentFrame = 0;
	finished = false;
}

const sf::IntRect& Animation::GetCurrentFrame() const
{
	return frames[currentFrame];
}

bool Animation::HasFinished() const
{
	return finished;
}

int Animation::GetFrameWidth() const
{
	return frames.empty() ? 0 : frames[0].size.x;
}

int Animation::GetFrameHeight() const
{
	return frames.empty() ? 0 : frames[0].size.y;
}
