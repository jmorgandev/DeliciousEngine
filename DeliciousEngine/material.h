#ifndef DELICIOUS_MATERIAL_H
#define DELICIOUS_MATERIAL_H

//Manages the attributes & uniforms of a shader.
#include "shader.h"
#include "dtypes.h"
#include "mesh.h"

//@TODO: Auto-generate global uniform block for standard engine assigned uniforms e.g:
// - matrix_mvp (Computed before sending to GPU)
// - matrix_view
// - matrix_projection
// - matrix_model
// - camera frustrum?
// - global lights? etc...

class Material {
public:

private:
	Shader* shader;
};

#endif