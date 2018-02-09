#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "glm.hpp"

class Camera;
class AABox;

class Frustum
{
private:
	enum Sides {NEAR_P = 0, FAR_P, LEFT_P, RIGHT_P, TOP_P, BOTTOM_P};
	Plane planes[6];
	Camera* camera;
	float zNear, zFar, fov, ratio;
	float farWidth, farHeight, nearWidth, nearHeight;
	glm::vec3 camPos;

	void calculateWidthAndHeight();
	void calculatePlanes();


public:
	Frustum(Camera* camera, float ratio);
	~Frustum();

	void init();
	void update(glm::vec3 camPos);
	
	bool checkBox(AABox &box);

};

#endif