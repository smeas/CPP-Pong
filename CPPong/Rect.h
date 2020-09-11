#pragma once
#include "Vec2.h"
#include <SDL2/SDL.h>

struct Rect
{
	float x, y;
	float width, height;

	Rect(float x, float y, float width, float height)
		: x(x), y(y),
		  width(width), height(height) {}

	Rect(const Vec2 &position, const Vec2 &size)
		: Rect(position.x, position.y, size.x, size.y) {}

	//explicit operator SDL_Rect() const
	//{
	//	return {
	//		(int)x, (int)y,
	//		(int)width, (int)height
	//	};
	//}

	operator SDL_FRect() const
	{
		return {x, y, width, height};
	}

	Vec2 Position() const { return {x, y}; }
	Vec2 Size() const { return { width, height }; }

	Vec2 Center() const
	{
		return { x + width / 2.0f, y + height / 2.0f };
	}

	float Left() const { return x; }
	float Top() const { return y; }
	float Right() const { return x + width; }
	float Bottom() const { return y + height; }

	void SetPosition(const Vec2 &pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void SetSize(const Vec2 &size)
	{
		width = size.x;
		height = size.y;
	}

	void SetCenter(const Vec2 &pos)
	{
		x = pos.x - width / 2.0f;
		y = pos.y - height / 2.0f;
	}

	void SetLeft(float value) { x = value; }
	void SetTop(float value) { y = value; }
	void SetRight(float value) { x = value - width; }
	void SetBottom(float value) { y = value - height; }

	
	bool Intersects(const Rect &other) const;
};
