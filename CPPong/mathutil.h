#pragma once

template <typename T>
constexpr T clamp(T value, T min, T max)
{
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

template <typename T>
constexpr T min(T a, T b)
{
	return a < b ? a : b;
}

template <typename T>
constexpr T max(T a, T b)
{
	return a > b ? a : b;
}

template <typename T>
constexpr T sq(T value)
{
	return value * value;
}

template <typename T>
constexpr T remap(T value, T low1, T high1, T low2, T high2)
{
	return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}
