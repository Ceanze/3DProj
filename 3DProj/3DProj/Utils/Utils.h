#ifndef UTILS_H
#define UTILS_H

#include "../glm/glm.hpp"

namespace Tools
{
	const float PI = 3.141592;

	static float interpolate(const float& x1, const float& x2, const float& t)
	{
		return x1*(1 - t) + x2 * t;
	}

	static float getSideX(const float& y, const float& angle)
	{
		return y / glm::tan(angle);
	}

	static float getSideY(const float& x, const float& angle)
	{
		return x * glm::tan(angle);
	}

	static float getHypFromCos(const float& x, const float& angle)
	{
		return x / glm::cos(angle);
	}

	static float getHypFromSin(const float& y, const float& angle)
	{
		return y / glm::sin(angle);
	}
	
	static float barryCentricHeight(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 xz)
	{
		float det = (v2.z - v3.z) * (v1.x - v3.x) - (v2.x - v3.x) * (v1.z - v3.z);
		float l1 = abs(((v2.z - v3.z) * (xz.x - v3.x) - (v2.x - v3.x) * (xz.y - v3.z)) / det);
		float l2 = abs(((v1.z - v3.z) * (xz.x - v3.x) - (v1.x - v3.x) * (xz.y - v3.z)) / det);
		float l3 = 1.0f - l1 - l2;
		return l1 * v1.y + l2 * v2.y + l3 * v3.y;

		

		//float det = (v3.x - v1.x) * (v2.z - v1.z) - (v3.z - v1.z) * (v2.x - v1.x);
		//float l1 = abs(((xz.x - v3.x) * (v2.z - v3.z) - (xz.y - v3.z) * (v2.x - v3.x)) / det);
		//float l2 = abs((xz.x - v3.x) * (v1.z - v3.z) - (xz.y - v3.z) * (v1.x - v3.x) / det);
		//float l3 = 1.0f - l1 - l2;

		//return l1 * v1.y + l2 * v2.y + l3 * v3.y;
	}

	static float getYaw(const glm::vec3& v)
	{
		return atan2(v.z, v.x);
	}

	static float getPitch(const glm::vec3& v)
	{
		return -(float)atan2(v.y, sqrt(v.x*v.x + v.z*v.z));
	}

	static float getRoll(const glm::vec3& v)
	{
		return (float)atan2(v.y, v.x);
	}
};

#endif