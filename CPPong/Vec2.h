#pragma once
#include <cmath>
#include "mathutil.h"

struct Vec2
{
	float x;
	float y;

	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}

	float Magnitude() const
	{
		return sqrt(sq(x) + sq(y));
	}

	void SetMagnitude(float value)
	{
		Normalize();
		*this *= value;
	}

	Vec2 Normalized() const
	{
		return *this / Magnitude();
	}

	void Normalize()
	{
		const float length = Magnitude();
		x /= length;
		y /= length;
	}


	Vec2 operator+(const Vec2 &other) const
	{
		return {x + other.x, y + other.y};
	}

	Vec2 operator-(const Vec2 &other) const
	{
		return {x - other.x, y - other.y};
	}

	Vec2 operator*(float value) const
	{
		return {x * value, y * value};
	}

	Vec2 operator/(float value) const
	{
		return {x / value, y / value};
	}

	Vec2& operator+=(const Vec2 &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator-=(const Vec2 &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator*=(float value)
	{
		x *= value;
		y *= value;
		return *this;
	}

	Vec2& operator/=(float value)
	{
		x /= value;
		y /= value;
		return *this;
	}


	static Vec2 FromAngle(float angle)
	{
		return {cos(angle), sin(angle)};
	}
};
