#include "dgl.h"
#include "dtypes.h"
#include "dcf.h"
#include "build_info.h"
#include <algorithm>

namespace dgl {

	bool compile(const GLuint& object, const std::string& source) {
		const GLchar* src = source.c_str();
		const GLint size = source.length();
		glShaderSource(object, 1, &src, NULL);
		glCompileShader(object);
		GLint status = 0;
		glGetShaderiv(object, GL_COMPILE_STATUS, &status);
		return (status != GL_FALSE);
	}

	bool compile(const GLuint& object, const GLchar* source) {
		const GLint size = dcf::str_len(source);
		glShaderSource(object, 1, &source, NULL);
		glCompileShader(object);
		GLint status = 0;
		glGetShaderiv(object, GL_COMPILE_STATUS, &status);
		return (status != GL_FALSE);
	}

	bool is_sampler(const GLuint& uniform_type) {
		//@Todo: Fix this lazy evaluation or find a better way to discern if a GLSL uniform
		// is a sampler or not
		return (uniform_type >= GL_SAMPLER_1D && uniform_type <= GL_UNSIGNED_INT_SAMPLER_2D_RECT);
	}
}