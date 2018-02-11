#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "../../Terrain/QuadTree.h"

#include "glm.hpp"

class Camera;
class AABox;

class Frustum
{
private:
	enum Sides {NEAR_P = 0, FAR_P, LEFT_P, RIGHT_P, TOP_P, BOTTOM_P};
	Plane planes[6];
	Camera* camera;
	QuadTree* quadTree;
	float zNear, zFar, fov, ratio;
	float farWidth, farHeight, nearWidth, nearHeight;
	glm::vec3 camPos;

	void calculateWidthAndHeight();
	void calculatePlanes();


public:
	Frustum(Camera* camera, QuadTree* quadTree, float ratio);
	~Frustum();

	void init();
	void update(glm::vec3 camPos);
	
	bool checkBox(AABox &box);

	float getZNear() const;
	float getZFar() const;
	float getNearHeight() const;
	float getFarHeight() const;
	float getNearWidth() const;
	float getFarWidth() const;

};

#endif