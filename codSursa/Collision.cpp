#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y)
		return true;

	return false;

	//varianta 2
	//return SDL_HasIntersection(&recA, &recB);
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		//std::cout << colA.tag << " hit " << colB.tag << '\n';
		return true;
	}
	return false;
}
