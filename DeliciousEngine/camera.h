#ifndef DELICIOUS_CAMERA_H
#define DELICIOUS_CAMERA_H

#include "system_var.h"
#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	void init(system_var* fov_var, system_var* aspect_var);
	void update();

	//@TODO: Pre-multiply the view and projection matrices. (Maybe)
	glm::mat4& transform_matrix()  { return transform;  }
	glm::mat4& view_matrix()	   { return view;       }
	glm::mat4& projection_matrix() { return projection; }

private:
	glm::mat4 view;
	glm::mat4 projection;

	//@TODO: Reference an entity transform rather than having a separate transform.
	glm::mat4 transform;

	system_var* field_of_view;
	system_var* aspect_ratio;
};

#endif