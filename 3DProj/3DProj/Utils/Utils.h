#include "../glm/glm.hpp"

namespace Tools
{
	const float PI = 3.141592;

	float interpolate(const float& x1, const float& x2, const float& t)
	{
		return x1*(1 - t) + x2 * t;
	}

	float getSideX(const float& y, const float& angle)
	{
		return y / glm::tan(angle);

		
	}

	float getSideY(const float& x, const float& angle)
	{
		return x * glm::tan(angle);
	}

	float getHypFromCos(const float& x, const float& angle)
	{
		return x / glm::cos(angle);
	}

	float getHypFromSin(const float& y, const float& angle)
	{
		return y / glm::sin(angle);
	}
	
	float barryCentricHeight(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 xz)
	{
		float height = 0;

		float dX1 = v2.x - v1.x;
		float dZ1 = v2.z - v1.z;

		float dX2 = v3.x - v1.x;;
		float dZ2 = v3.z - v1.z;;


		float a2 = (((xz.y * dZ1) / dX1) - xz.y) / (-dZ2 + ((dX2*dZ1)/dX1));
		float a1 = ((xz.y - a2*dZ2) / dZ1);

		return height;
	}
};