#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H
#include <SDL_video.h>
#include <SDL_mouse.h>
#include <glew.h>

#include "system.h"
#include "system_var.h"
#include "camera.h"
#include "build_info.h"
#include <imgui.h>

//@Temp
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

class DeliciousEngine;

class Screen : public System {
public:
	Screen(DeliciousEngine& engine);
	Screen() = delete;

	bool load() override;
	bool start() override { return create_window(); }
	bool free() override;

	void render_frame();
	
	bool create_window();
	bool reload_window();

	int width() { return vid_width.as_int; }
	int height() { return vid_height.as_int; }
	float aspect_ratio() {
		return (float)vid_width.as_int / (float)vid_height.as_int;
	}
	ImVec2 imgui_size(float scale = 1.0f) {
		return ImVec2((float)vid_width.as_int * scale, (float)vid_height.as_int * scale);
	}
	ImVec2 imgui_center() {
		return ImVec2((float)vid_width.as_int / 2, (float)vid_height.as_int / 2);
	}

	void resize(int width, int height);

	void begin_gui();

	Camera* get_camera() { return &camera; }
private:
	SDL_Window*   window;
	SDL_GLContext gl_context;

	SDL_Cursor*   gui_cursors[ImGuiMouseCursor_Count_];

	SystemVar vid_width;
	SystemVar vid_height;
	SystemVar vid_fullscreen;
	SystemVar vid_borderless;
	SystemVar vid_fov;
	
	float near_plane;
	float far_plane;

	Camera camera;

	void draw_imgui();
	//@Todo: Put these floating variables into some sort of struct...
	GLuint gui_vao;
	GLuint gui_vbo;
	GLuint gui_ebo;
	GLuint gui_shader_handle;
	GLuint gui_texture_handle;
	GLint  gui_tex_uniform;
	GLint  gui_proj_uniform;
	GLint  gui_pos_attrib;
	GLint  gui_texcoord_attrib;
	GLint  gui_color_attrib;

	void create_gui_objects();
	void invalidate_gui_objects();

	glm::mat4 ortho_matrix;
};

#endif