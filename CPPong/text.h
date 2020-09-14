#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Vec2.h"

enum TextAlign
{
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER,
};

struct Text
{
	SDL_Texture *texture;
	SDL_Rect rect;

	Text() = default;

	Text(SDL_Texture *texture, const SDL_Rect &rect)
		: texture(texture),
		  rect(rect) {}

	Text(const Text &other) = delete;

	Text(Text &&other) noexcept
		: texture(other.texture),
		  rect(other.rect)
	{
		other.texture = nullptr;
	}

	Text& operator=(const Text &other) = delete;

	Text& operator=(Text &&other) noexcept
	{
		if (this == &other)
			return *this;
		std::swap(texture, other.texture);
		rect = other.rect;
		return *this;
	}

	~Text()
	{
		if (texture != nullptr)
		{
			SDL_DestroyTexture(texture);
		}
	}
};

inline TTF_Font* LoadFont(const char *path, int ptSize)
{
	TTF_Font *font = TTF_OpenFont(path, ptSize);
	if (font == nullptr)
	{
		SDL_Log("Failed to open font \"%s\": %s", path, TTF_GetError());
	}

	return font;
}

inline Text CreateText(
	SDL_Renderer *renderer,
	TTF_Font *font,
	const std::string &text,
	const SDL_Color &color = {255, 255, 255, 255})
{
	SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (surface == nullptr)
	{
		SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
		return {};
	}

	// BUG: This seems to cause a tiny memory leak in even though I'm freeing it properly. Bug in SDL?
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr)
	{
		SDL_Log("SDL_CreateTextureFromSurface failed: %s", TTF_GetError());
		SDL_FreeSurface(surface);
		return {};
	}

	const SDL_Rect rect = {0, 0, surface->w, surface->h};
	SDL_FreeSurface(surface);

	return {texture, rect};
}

inline void DrawText(
	SDL_Renderer *renderer,
	const Text &text,
	const Vec2 &position,
	const TextAlign alignment = ALIGN_LEFT)
{
	SDL_FRect destRect = {position.x, position.y, (float)text.rect.w, (float)text.rect.h};

	if (alignment == ALIGN_RIGHT)
		destRect.x -= (float)text.rect.w;
	else if (alignment == ALIGN_CENTER)
		destRect.x -= (float)text.rect.w / 2.0f;

	SDL_RenderCopyF(renderer, text.texture, &text.rect, &destRect);
}

inline void DrawText(
	SDL_Renderer *renderer,
	TTF_Font *font,
	const std::string &string,
	const Vec2 &position,
	const TextAlign alignment = ALIGN_LEFT,
	const SDL_Color &color = {255, 255, 255, 255})
{
	const Text text = CreateText(renderer, font, string, color);
	DrawText(renderer, text, position, alignment);
}
