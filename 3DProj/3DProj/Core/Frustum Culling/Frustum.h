#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "../Config.h"

class Camera;
class AABox;

class Frustum
{
private:
	enum {NEAR = 0, FAR, LEFT, RIGHT, TOP, BOTTOM};
	Plane planes[6];
	Camera* camera;
	float zNear, zFar, fov, ratio;
	float farWidth, farHeight, nearWidth, nearHeight;
	glm::vec3 camPos;

	void calculateWidthAndHeight();
	void calculatePlanes();


public:
	Frustum(Camera* camera, float zNear, float zFar, float ratio);
	~Frustum();

	void init(glm::vec3 camPos);
	void update(glm::vec3 camPos);
	
	bool checkBox(AABox &box);
};

#endif