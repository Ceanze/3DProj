#ifndef CONFIG_H
#define CONFIG_H

// ------------------ INCLUDES ------------------
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

// -------------- DEFAULT SETTINGS --------------
#define DEFAULT_WIDTH 840
#define DEFAULT_HEIGHT 680
#define ZNEAR 0.1f
#define ZFAR 100.0f

// ------------------- MODEL --------------------
#define MODEL_PATH "./Resources/Models/"
#define DEFAULT_MATERIAL_NAME "DefaultMTL"

// ------------------ SHADER --------------------
#define SHADER_PATH "./Resources/Shaders/"

// ------------------ GLOBALS -------------------
static const glm::vec3 GLOBAL_UP_VECTOR = glm::vec3{ 0.0f, 1.0f, 0.0f };

#endif
