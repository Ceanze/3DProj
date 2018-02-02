#ifndef CONFIG_H
#define CONFIG_H

// ------------------ INCLUDES ------------------
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

// ------------------- Modes --------------------
#define RENDER_GUI

// -------------- DEFAULT SETTINGS --------------
#define DEFAULT_WIDTH			840
#define DEFAULT_HEIGHT			680
#define ZNEAR					0.1f
#define ZFAR					100.0f

// ------------------- MODEL --------------------
#define MODEL_PATH				"./Resources/Models/"
#define DEFAULT_MATERIAL_NAME	"DefaultMTL"
#define DEFAULT_TEXTURE_PATH	"./Resources/Misc/DefaultTexture2.png"
#define DEFAULT_NORMAL_MAP_PATH	"./Resources/Misc/DefaultNormalMap2.png"

// ------------------ SHADER --------------------
#define SHADER_PATH				"./Resources/Shaders/"

// ------------------ GLOBALS -------------------
static const glm::vec3 GLOBAL_UP_VECTOR = glm::vec3{ 0.0f, 1.0f, 0.0f };

// ---------- TEXTURE LOADING CONSTANTS ---------
#define TEXTURE_FAILED		0x0
#define TEXTURE_SUCCEEDED	0x1
#define TEXTURE_PASSED		0x2

// ------------ OBJ LOADER CONSTANTS ------------
#define USE_NORMAL_MAP		0x1
#define FLIP_UV_Y			0x2
#define FLIP_UV_X			0x4

#endif
