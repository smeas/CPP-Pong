#include "Ball.h"

static const float oneOverSqrt2 = (float)(1.0 / sqrt(2.0));

void Ball::Draw(SDL_Renderer *renderer)
{
	SDL_FRect rect = static_cast<SDL_FRect>(mRect);
	SDL_Rect srcRect = {};

	SDL_QueryTexture(mTexture, nullptr, nullptr, &srcRect.w, &srcRect.h);
	SDL_RenderCopyF(renderer, mTexture, &srcRect, &rect);
}

void Ball::Move(float dt)
{
	mRect.SetPosition(mRect.Position() + velocity * dt);
}

bool Ball::WallCollision(float width, float height)
{
	if (mRect.Top() < 0)
	{
		mRect.SetTop(0);
		velocity.y = -velocity.y;
		return true;
	}

	if (mRect.Bottom() >= height)
	{
		mRect.SetBottom(height - 1);
		velocity.y = -velocity.y;
		return true;
	}

	/*if (mRect.Left() < 0)
	{
		mRect.SetLeft(0);
		velocity.x = -velocity.x;
	}

	if (mRect.Right() >= width)
	{
		mRect.SetRight(width - 1);
		velocity.x = -velocity.x;
	}*/

	return false;
}

float Shortest(float a, float b)
{
	if (abs(a) < abs(b)) return a;
	return b;
}

bool Ball::PaddleCollision(const Rect &paddle)
{
	if (!mRect.Intersects(paddle)) // early out
		return false;

	float dx;
	float dy = Shortest(mRect.Bottom() - paddle.Top(), mRect.Top() - paddle.Bottom());

	const float t = clamp(remap(mRect.Center().y, paddle.Bottom(), paddle.Top(), oneOverSqrt2, -oneOverSqrt2),
	                      -oneOverSqrt2, oneOverSqrt2);
	const float reflectionAngle = asin(t);


	if (mRect.Right() > paddle.Left() // left
		&& mRect.Right() < paddle.Right())
	{
		dx = mRect.Right() - paddle.Left();

		velocity = Vec2::FromAngle(reflectionAngle) * velocity.Magnitude();
		velocity.x = -velocity.x;
	}
	else if (mRect.Left() < paddle.Right() // right
		&& mRect.Left() > paddle.Left()) 
	{
		dx = mRect.Left() - paddle.Right();

		velocity = Vec2::FromAngle(reflectionAngle) * velocity.Magnitude();
	}
	else
	{
		return false;
	}

	// Push the ball out of the paddle.
	if (abs(dx) < abs(dy))
		mRect.x -= dx;
	else
		mRect.y -= dy;

	return true;
}


//bool Ball::PaddleCollision(const Rect &paddle)
//{
//	if (!mRect.Intersects(paddle))
//		return false;
//
//	// Top
//	if (mRect.Bottom() > paddle.Top()
//		&& mRect.Center().y < paddle.Top())
//	{
//		mRect.SetBottom(paddle.Top());
//		velocity.y = -abs(velocity.y);
//		return true;
//	}
//
//	// Bottom
//	if (mRect.Top() < paddle.Bottom()
//		&& mRect.Center().y > paddle.Bottom())
//	{
//		mRect.SetTop(paddle.Bottom());
//		velocity.y = abs(velocity.y);
//		return true;
//	}
//
//	// Left
//	if (mRect.Right() > paddle.Left()
//		&& mRect.Right() < paddle.Right())
//	{
//		mRect.SetRight(paddle.Left());
//
//		const float t = clamp(remap(mRect.Center().y, paddle.Bottom(), paddle.Top(), oneOverSqrt2, -oneOverSqrt2),
//		                      -oneOverSqrt2, oneOverSqrt2);
//		const float reflectionAngle = asin(t);
//		velocity = Vec2::FromAngle(reflectionAngle) * velocity.Magnitude();
//		velocity.x = -velocity.x;
//		return true;
//	}
//
//	// Right
//	if (mRect.Left() < paddle.Right()
//		&& mRect.Left() > paddle.Left())
//	{
//		mRect.SetLeft(paddle.Right());
//
//		const float t = clamp(remap(mRect.Center().y, paddle.Bottom(), paddle.Top(), oneOverSqrt2, -oneOverSqrt2),
//		                      -oneOverSqrt2, oneOverSqrt2);
//		const float reflectionAngle = asin(t);
//		velocity = Vec2::FromAngle(reflectionAngle) * velocity.Magnitude();
//		return true;
//	}
//
//	return false;
//}

//void Ball::PaddleCollision(const Rect &paddle)
//{
//	if (!mRect.Intersects(paddle))
//		return;
//
//	float timeSinceIntersectHorizontal;
//	float timeSinceIntersectVertical;
//
//	if (velocity.x > 0)
//		timeSinceIntersectHorizontal = (mRect.Right() - paddle.Left()) / velocity.x;
//	else
//		timeSinceIntersectHorizontal = (mRect.Left() - paddle.Right()) / velocity.x;
//
//	if (velocity.y > 0)
//		timeSinceIntersectVertical = (mRect.Bottom() - paddle.Top()) / velocity.y;
//	else
//		timeSinceIntersectVertical = (mRect.Top() - paddle.Bottom()) / velocity.y;
//
//	//// Move the ball back to where it collided with the paddle.
//	//const float timeStep = min(timeSinceIntersectVertical, timeSinceIntersectHorizontal);
//	//mRect.x -= velocity.x * timeStep;
//	//mRect.y -= velocity.y * timeStep;
//
//	if (timeSinceIntersectVertical < timeSinceIntersectHorizontal)
//	{
//
//		
//		velocity.y = -velocity.y;
//	}
//	else
//	{
//		float t = remap(mRect.Center().y, paddle.Bottom(), paddle.Top(), 1.f, -1.f);
//		t = clamp(t, -1.f, 1.f);
//		const float reflectionAngle = asin(t);
//		velocity = Vec2::FromAngle(reflectionAngle) * velocity.Magnitude();
//		
//		if (velocity.x <= 0)
//			velocity.x = -velocity.x;
//	}
//
//	//// Move the ball forward to where it should be.
//	//mRect.x += velocity.x * timeStep;
//	//mRect.y += velocity.y * timeStep;
//}
