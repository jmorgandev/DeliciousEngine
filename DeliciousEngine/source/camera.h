#ifndef DELICIOUS_CAMERA_H
#define DELICIOUS_CAMERA_H

#include "system_var.h"
#include <mat4x4.hpp>

class Camera {
public:
	enum Mode { Perspective, Orthographic };

	Camera();
	void update_matrices();
	void look_at(glm::vec3 pos);

	inline float get_fov()         const { return fov; }
	inline float get_nearclip()    const { return nearclip; }
	inline float get_farclip()     const { return farclip; }
	inline bool  is_orthographic() const { return ortho; }

	inline void set_fov(float degrees)       { fov = degrees; }
	inline void set_nearclip(float distance) { nearclip = distance; }
	inline void set_farclip(float distance)  { farclip = distance; }

	inline void use_perspective()  { ortho = false; }
	inline void use_orthographic() { ortho = true; }

	//@Todo: Pre-multiply the view and projection matrices.
	glm::mat4& transform_matrix()  { return transform; }
	glm::mat4& view_matrix()	   { return view; }
	glm::mat4& projection_matrix() { return projection; }

private:
	glm::mat4 view;
	glm::mat4 projection;

	//@Todo: Reference an entity transform rather than having a separate transform.
	// Or have both a way to attach to entity, and a camera transform? Maybe.
	glm::mat4 transform;

	float fov;
	float nearclip;
	float farclip;
	bool  ortho;
};

#endif