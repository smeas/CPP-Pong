#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Ball.h"
#include "Clock.h"
#include "Paddle.h"
#include "text.h"

enum Side
{
	SIDE_LEFT = 0,
	SIDE_RIGHT = 1
};

class Game
{
public:
	static constexpr float cPaddleSpeed = 240.0f;
	static constexpr float cBallSpeed = 340.0f;
	static constexpr float cSpeedIncreasePerBounce = 8.0f;

	static Game *sInstance;

	int mWidth;
	int mHeight;
	Rect mRect;
	
private:
	Clock mClock;
	SDL_Window *mWindow = nullptr;
	SDL_Renderer *mRenderer = nullptr;
	SDL_Surface *mSurface = nullptr;

	Paddle mPaddle1;
	Paddle mPaddle2;
	Ball mBall;

	// resources
	SDL_Texture *mPaddleTextures[2] = {};
	SDL_Texture *mBallTexture = nullptr;
	TTF_Font *mBitFont = nullptr;
	Mix_Chunk *mSounds[3] = {};

	// state
	bool mPaused = false;
	int mScore[2] = {};
	Side mLastScoringSide = SIDE_LEFT;
	Side mLastSpeedupSide = SIDE_LEFT;

public:
	Game(int width, int height)
		: mWidth(width), mHeight(height),
		  mRect(Rect(0, 0, (float)width, (float)height)),
		  mPaddle1(Rect(10, 10, 40, 200)),
		  mPaddle2(Rect(10, 10, 40, 200)),
		  mBall(Rect(400, 300, 40, 40))
	{
		sInstance = this;
		mBall.mRect.SetCenter({ (float)mWidth / 2, (float)mHeight / 2 });
		mPaddle1.mRect.y = ((float)mHeight - mPaddle1.mRect.height) / 2.0f;
		mPaddle2.mRect.y = ((float)mHeight - mPaddle2.mRect.height) / 2.0f;
		mPaddle2.mRect.SetRight((float)mWidth - 10);
		Reset();
	}

	void Run();

private:
	void InitializeSDL();
	void LoadResources();
	void Cleanup();

	void EventLoop();

	void HandleKeydown(const SDL_KeyboardEvent &event);
	void Update();
	void Draw();
	void Reset();

	void BeginRound();
};
