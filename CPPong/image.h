#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//inline SDL_Surface* LoadImage(const char *path)
//{
//	SDL_Surface *surface = IMG_Load(path);
//	if (surface == nullptr)
//	{
//		SDL_Log("Failed to load image \"%s\": %s", path, IMG_GetError());
//	}
//
//	return surface;
//}
//
//inline SDL_Surface* LoadOptimizedImage(const char *path, const SDL_Surface *screenSurface)
//{
//	SDL_Surface *surface = LoadImage(path);
//	if (surface == nullptr)
//		return nullptr;
//
//	SDL_Surface *optimizedSurface = SDL_ConvertSurface(surface, screenSurface->format, 0);
//	if (optimizedSurface == nullptr)
//	{
//		SDL_Log("Failed to optimize image surface \"%s\": %s", path, SDL_GetError());
//	}
//
//	SDL_FreeSurface(surface);
//	return optimizedSurface;
//}

inline SDL_Texture* LoadTexture(SDL_Renderer *renderer, const char *path)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path);
	if (texture == nullptr)
	{
		SDL_Log("Failed to load texture \"%s\": %s", path, IMG_GetError());
	}

	return texture;
}
