#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

inline Mix_Chunk* LoadAudio(const char *path)
{
	Mix_Chunk *chunk = Mix_LoadWAV(path);
	if (chunk == nullptr)
	{
		SDL_Log("Failed to load audio \"%s\": %s", path, Mix_GetError());
	}

	return chunk;
}
