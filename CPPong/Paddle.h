#pragma once
#include <SDL2/SDL.h>

#include "Thing.h"

class Paddle : public Thing
{
	static constexpr float speed = 256.0f;

public:
	SDL_Texture *mTexture = nullptr;

	using Thing::Thing;

	void Draw(SDL_Renderer *renderer) override;

	void Move(float dt, float direction);
};
