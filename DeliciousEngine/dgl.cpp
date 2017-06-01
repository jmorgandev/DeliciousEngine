#include "dgl.h"

namespace dgl {

	bool compile(const GLuint& object, const std::string& source) {
		GLint type = 0;
		glGetShaderiv(object, GL_SHADER_TYPE, &type);
		const GLchar* src = source.c_str();
		const GLint size = source.length();
		glShaderSource(type, 1, &src, &size);
		glCompileShader(object);
		GLint status = 0;
		glGetShaderiv(object, GL_COMPILE_STATUS, &status);
		return (status != GL_FALSE);
	}

	

}