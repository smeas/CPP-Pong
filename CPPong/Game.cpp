#include "Game.h"

#include "image.h"
#include "sound.h"

Game *Game::sInstance;

void Game::Run()
{
	InitializeSDL();
	LoadResources();
	EventLoop();
}

void Game::InitializeSDL()
{
	// Initialize SDL & Co.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		exit(1);
	}

	const int imgInitFlags = IMG_INIT_PNG;
	if (IMG_Init(imgInitFlags) != imgInitFlags)
	{
		SDL_Log("IMG_Init failed: %s", IMG_GetError());
		exit(1);
	}

	if (TTF_Init() != 0)
	{
		SDL_Log("TTF_Init failed: %s", SDL_GetError());
		exit(1);
	}

	// Create the window.
	mWindow = SDL_CreateWindow(
		"C++ Pong",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		mWidth, mHeight, 0);

	if (mWindow == nullptr)
	{
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		exit(1);
	}

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // enable linear filtering

	// Setup a renderer for the window.
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		SDL_Log("SDL_GetRenderer failed: %s", SDL_GetError());
		exit(1);
	}

	mSurface = SDL_GetWindowSurface(mWindow);
	if (mSurface == nullptr)
	{
		SDL_Log("SDL_GetWindowSurface failed: %s", SDL_GetError());
		exit(1);
	}

	// Setup audio.
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 4096) != 0)
	{
		SDL_Log("Mix_OpenAudio failed: %s", Mix_GetError());
		exit(1);
	}

	Mix_Volume(0, MIX_MAX_VOLUME / 4);
}

void Game::LoadResources()
{
	mPaddleTextures[0] = LoadTexture(mRenderer, "Resources/paddle1.png");
	mPaddleTextures[1] = LoadTexture(mRenderer, "Resources/paddle2.png");
	mBallTexture = LoadTexture(mRenderer, "Resources/ball.png");
	mBitFont = LoadFont("Resources/bit5x3.ttf", 72);
	mSounds[0] = LoadAudio("Resources/pongblip1.wav");
	mSounds[1] = LoadAudio("Resources/pongblip2.wav");
	mSounds[2] = LoadAudio("Resources/pongblip3.wav");

	if (mPaddleTextures[0] == nullptr
		|| mPaddleTextures[1] == nullptr
		|| mBallTexture == nullptr
		|| mBitFont == nullptr
		|| mSounds[0] == nullptr
		|| mSounds[1] == nullptr
		|| mSounds[2] == nullptr)
	{
		exit(1);
	}

	mPaddle1.mTexture = mPaddleTextures[0];
	mPaddle2.mTexture = mPaddleTextures[1];
	mBall.mTexture = mBallTexture;
}

void Game::Cleanup()
{
	SDL_DestroyTexture(mPaddleTextures[0]);
	SDL_DestroyTexture(mPaddleTextures[1]);
	SDL_DestroyTexture(mBallTexture);

	TTF_CloseFont(mBitFont);

	Mix_CloseAudio();
	Mix_FreeChunk(mSounds[0]);
	Mix_FreeChunk(mSounds[1]);
	Mix_FreeChunk(mSounds[2]);

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	TTF_Quit();
	SDL_Quit();
}

void Game::EventLoop()
{
	bool quit = false;
	SDL_Event event;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_KEYDOWN:
					HandleKeydown(event.key);
					break;
			}
		}

		mClock.Tick();
		Draw();
		SDL_RenderPresent(mRenderer);
	}
}

void Game::HandleKeydown(const SDL_KeyboardEvent &event)
{
	if (event.keysym.sym == SDLK_ESCAPE)
	{
		mPaused = !mPaused;
	}
}

void Game::Update()
{
	const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

	// Movement
	if (keyState[SDL_SCANCODE_W])
		mPaddle1.Move(mClock.DeltaTime(), -1);
	if (keyState[SDL_SCANCODE_S])
		mPaddle1.Move(mClock.DeltaTime(), 1);

	if (keyState[SDL_SCANCODE_UP])
		mPaddle2.Move(mClock.DeltaTime(), -1);
	if (keyState[SDL_SCANCODE_DOWN])
		mPaddle2.Move(mClock.DeltaTime(), 1);

	mBall.Move(mClock.DeltaTime());


	// Collision
	if (mBall.PaddleCollision(mPaddle1.mRect))
	{
		if (mLastSpeedupSide != SIDE_LEFT)
		{
			mBall.velocity.SetMagnitude(mBall.velocity.Magnitude() + cSpeedIncreasePerBounce);
			mLastSpeedupSide = SIDE_LEFT;
		}

		Mix_PlayChannel(0, mSounds[0], 0);
	}
	else if (mBall.PaddleCollision(mPaddle2.mRect))
	{
		if (mLastSpeedupSide != SIDE_RIGHT)
		{
			mBall.velocity.SetMagnitude(mBall.velocity.Magnitude() + cSpeedIncreasePerBounce);
			mLastSpeedupSide = SIDE_RIGHT;
		}

		Mix_PlayChannel(0, mSounds[1], 0);
	}

	if (mBall.WallCollision((float)mWidth, (float)mHeight))
	{
		Mix_PlayChannel(0, mSounds[2], 0);
	}


	// Scoring
	if (mBall.mRect.Left() >= mWidth)
	{
		mScore[SIDE_LEFT] += 1;
		mLastScoringSide = SIDE_LEFT;
		BeginRound();
	}
	else if (mBall.mRect.Right() < 0)
	{
		mScore[SIDE_RIGHT] += 1;
		mLastScoringSide = SIDE_RIGHT;
		BeginRound();
	}
}

void Game::Draw()
{
	if (!mPaused)
	{
		Update();
	}

	SDL_SetRenderDrawColor(mRenderer, 52, 201, 235, 255);
	SDL_RenderClear(mRenderer);

	// Draw mid line.
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 255);
	SDL_RenderDrawLineF(mRenderer, (float)mWidth / 2, 0, (float)mWidth / 2, (float)mHeight);

	mPaddle1.Draw(mRenderer);
	mPaddle2.Draw(mRenderer);
	mBall.Draw(mRenderer);

	// Draw score.
	{
		const Vec2 topCenter = Vec2(mWidth / 2.0f, 10);
		DrawText(mRenderer, mBitFont, std::to_string(mScore[SIDE_LEFT]), topCenter - Vec2(15, 0), ALIGN_RIGHT);
		DrawText(mRenderer, mBitFont, std::to_string(mScore[SIDE_RIGHT]), topCenter + Vec2(25, 0), ALIGN_LEFT);
	}

	if (mPaused)
	{
		const Text text = CreateText(mRenderer, mBitFont, "Paused");
		DrawText(mRenderer, text, Vec2((mWidth - text.rect.w) / 2.0f + 5, (mHeight - text.rect.h) / 2.0f));
	}
}

void Game::Reset()
{
	mScore[0] = 0;
	mScore[1] = 0;
	mLastScoringSide = SIDE_LEFT;
	mLastSpeedupSide = SIDE_LEFT;
	BeginRound();
}

void Game::BeginRound()
{
	const float direction = mLastScoringSide == SIDE_LEFT ? -1.0f : 1.0f;

	mBall.mRect.SetCenter(Vec2((float)mWidth / 2, (float)mHeight / 2));
	mBall.velocity = Vec2(direction, .1f).Normalized() * cBallSpeed;
}
