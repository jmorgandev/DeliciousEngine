#ifndef DELICIOUS_SHADER_H
#define DELICIOUS_SHADER_H

#include <GL/glew.h>

class Shader {
public:
	Shader(GLuint object_in);
	~Shader();

	void	bind();

	GLuint	get_gpu_object();
private:
	GLuint gpu_object;
};

#endif