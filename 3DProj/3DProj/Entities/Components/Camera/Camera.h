#ifndef CAMERA_H
#define CAMERA_H

#include "../Component.h"
#include "../../../Core/Config.h"
#include "../../../Utils/Transform.h"
//#include "../../../Core/Frustum Culling/Frustum.h"

#include <glm.hpp>

class Camera : public Component
{
public:
	Camera(Display* display, glm::vec3 relativePosition = glm::vec3(0.f, 0.f, 0.f), float fov = 0.7853975f, float zNear = ZNEAR, float zFar = ZFAR);
	Camera(Display* display, float width, float height, glm::vec3 relativePosition = glm::vec3(0.f, 0.f, 0.f), float zNear = ZNEAR, float zFar = ZFAR);
	virtual ~Camera();

	void init();
	void update(const float& dt);
	void input(Display* display);

	void lookAt(const glm::vec3& target);
	void setRelativePosition(const glm::vec3& relativePosition);
	void rotate(float yaw, float pitch, float roll);

	void updateProj();

	void activate();
	void deactivate();

	void setDirection(const glm::vec3& direction);
	bool isCamOrtho() const;

	float getFOV() const;
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;
	glm::mat4 getVP() const;
	float getFarPlane() const;
	float getNearPlane() const;

private:
	void updateProj(float fov, float zNear, float zFar);
	void updateView(const glm::vec3& f, const glm::vec3& r, const glm::vec3& u, const glm::vec3& pos);
	void setLocalPositionMat();
	void setWorldPosition();
	Transform localTransform;

	Display* display;

	glm::mat4 localPositionMat;
	glm::mat4 proj;
	glm::mat4 view;
	glm::vec3 worldPosition;
	glm::vec3 relativePosition;
	glm::vec3 yawPitchRoll;
	glm::vec3 f, r, u;
	float fov, zNear, zFar;
	float dt;
	float width, height;

	bool isCClicked;
	bool isCPressed;
	bool active;
	bool orthoCam;
};

#endif
