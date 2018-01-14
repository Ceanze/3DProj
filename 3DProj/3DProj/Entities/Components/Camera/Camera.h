#ifndef CAMERA_H
#define CAMERA_H

#include "../Component.h"
#include "../../../Core/Config.h"

#include <glm.hpp>

class Camera : public Component
{
public:
	Camera(Display* display, const glm::vec3& position, float fov = 0.7853975f, float zNear = ZNEAR, float zFar = ZFAR);
	virtual ~Camera();

	void init();
	void update(const float& dt);
	void input(Display* display);

	void lookAt(const glm::vec3& target);
	void setPosition(const glm::vec3& position);
	void move(const glm::vec3& offset);
	void rotate(float yaw, float pitch, float roll);

	void updateProj();

	float getFOV() const;
	glm::vec3 getPosition();
	glm::mat4 getVP();

private:
	void updateProj(float fov, float zNear, float zFar);
	void updateView(const glm::vec3& f, const glm::vec3& r, const glm::vec3& u, const glm::vec3& pos);

	Display* display;

	glm::mat4 proj;
	glm::mat4 view;
	glm::vec3 position;
	glm::vec3 yawPitchRoll;
	glm::vec3 f, r, u;
	float fov, zNear, zFar;
	float dt;
};

#endif
