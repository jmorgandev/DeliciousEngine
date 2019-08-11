#ifndef DELICIOUS_CAMERA_H
#define DELICIOUS_CAMERA_H

#include <mat4x4.hpp>

class Camera {
public:
	enum Mode { Perspective, Orthographic };

	Camera();
	void update_projection(float scr_w, float scr_h, float scr_aspect);
	void look_at(glm::vec3 pos);

	float get_fov() const { return fov; }
	float get_nearclip() const { return nearclip; }
	float get_farclip() const { return farclip; }
	bool  is_orthographic() const { return ortho; }

	void set_fov(float fov) { this->fov = fov; }
	void set_nearclip(float nearclip) { this->nearclip = nearclip; }
	void set_farclip(float farclip) { this->farclip = farclip; }

	void use_perspective()  { ortho = false; }
	void use_orthographic() { ortho = true; }

	//@Todo: Pre-multiply the view and projection matrices.
	glm::mat4& transform_matrix()  { return transform; }
	glm::mat4& projection_matrix() { return projection; }
	glm::mat4& view_matrix() { return view; }

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