#pragma once
#include "Rect.h"
#include "Thing.h"

class Ball : public Thing
{
public:
	Vec2 velocity;
	SDL_Texture *mTexture = nullptr;

	using Thing::Thing;

	void Draw(SDL_Renderer *renderer) override;

	void Move(float dt);
	bool WallCollision(float width, float height);
	bool PaddleCollision(const Rect &paddle);
};
