#include "Rect.h"

bool Rect::Intersects(const Rect &other) const
{
	if (Left() >= other.Right() || other.Left() >= Right())
		return false;

	if (Top() >= other.Bottom() || other.Top() >= Bottom())
		return false;

	return true;
}
