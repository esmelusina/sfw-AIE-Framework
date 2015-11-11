#include <cmath>
#include "collisiontests.h"
#include "GameObject.h"

float itest_Circle(float x1, float y1, float r1,
				   float x2, float y2, float r2)
{
	return (r1 + r2) - sqrtf((x1 - x2)*(x1 - x2) 
						   + (y1 - y2)*(y1 - y2));
}

float itest_AABB(float x1, float y1, float hw1, float hh1,
		    	 float x2, float y2, float hw2, float hh2)
{
	float b1 = y1 - hh1;
	float t1 = y1 + hh1;
	float l1 = x1 - hw1;
	float r1 = x1 + hw1;

	float b2 = y2 - hh2;
	float t2 = y2 + hh2;	
	float l2 = x2 - hw2;
	float r2 = x2 + hw2;

	float xt = fmin(r1 - l2, r2 - l1);
	float yt = fmin(t1 - b2, t2 - b1);

	return fmin(xt, yt);
}

float clamp(float min, float max, float val)
{
	return fmax(fmin(max, val), min);
}

float itest_CircleAABB(float x1, float y1, float r1,
					   float x2, float y2, float hw2, float hh2)
{
	// find point on the rectangle that's closest to the circle
	float px = clamp(x2 - hw2, x2 + hw2, x1);
	float py = clamp(y2 - hh2, y2 + hh2, y1);

	return itest_Circle(x1, y1, r1, px, py, 0);
}

float doCollision(GameObject &go1, GameObject &go2)
{
	float distance = itest_AABB(go1.x, go1.y, go1.width / 2, go1.height / 2, go2.x, go2.y, go2.width / 2, go2.height / 2);
	if (distance > 0)
	{
		go1.onCollision(go2, distance);
		go2.onCollision(go1, distance);
	}
	return distance;
}