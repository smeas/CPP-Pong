#pragma once
#include "Rect.h"

class Thing
{
public:
	Rect mRect;

	Thing() : Thing(Rect(0, 0, 0, 0)) {}

	explicit Thing(const Rect &rect)
		: mRect(rect) {}

	virtual void Draw(SDL_Renderer *renderer) = 0;

	virtual ~Thing() = default;
};
