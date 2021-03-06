#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <string>

class Shader
{
public:
	Shader();
	Shader(const std::string& name, GLuint type);
	~Shader();

	void create(const std::string& name, GLuint type);
	void compile();

	const GLuint getID() const;
	std::string getName() const;
	GLuint getType() const;

private:
	GLuint id;
	std::string name;
	GLuint type;
};

#endif