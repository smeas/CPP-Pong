#include "Paddle.h"

#include "Game.h"
#include "mathutil.h"

void Paddle::Move(float dt, float direction)
{
	mRect.y = clamp(mRect.y + speed * direction * dt, 0.0f, (float)Game::sInstance->mHeight - mRect.height);
}

void Paddle::Draw(SDL_Renderer *renderer)
{
	const SDL_FRect rect = static_cast<SDL_FRect>(mRect);
	SDL_Rect srcRect = {};

	SDL_QueryTexture(mTexture, nullptr, nullptr, &srcRect.w, &srcRect.h);
	SDL_RenderCopyF(renderer, mTexture, &srcRect, &rect);
}
