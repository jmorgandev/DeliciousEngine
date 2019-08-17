#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL_video.h>
#include <SDL_mouse.h>
#include <glad.h>

#include "module.h"
#include "camera.h"
#include "build_info.h"
#include <imgui.h>

//@Temp
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

class DeliciousEngine;

class Screen : public Module {
public:
	Screen(DeliciousEngine& engine);
	Screen() = delete;

	bool load() override;
	bool start() override { return create_window(); }
	bool free() override;

	void render_frame();
	
	bool create_window();
	bool reload_window();

	int width() { return window_width; }
	int height() { return window_height; }

	float aspect_ratio() {
		return (float)window_width / (float)window_height;
	}
	ImVec2 imgui_size(float scale = 1.0f) {
		return ImVec2((float)window_width * scale, (float)window_height * scale);
	}
	ImVec2 imgui_center() {
		return ImVec2((float)window_width / 2, (float)window_height / 2);
	}

	void resize(int width, int height);

	void begin_gui();

	Camera* get_camera() { return &camera; }
private:
	SDL_Window*   window;
	SDL_GLContext gl_context;

	SDL_Cursor*   gui_cursors[ImGuiMouseCursor_COUNT];

	int window_width, window_height;
	bool fullscreen, borderless;
	
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

	Uint64 last_time;
};

#endif