#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H
#include <SDL_video.h>
#include <SDL_mouse.h>
#include <glew.h>

#include "system_var.h"
#include "camera.h"
#include "build_info.h"
#include <imgui.h>

//@TEMP
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

class Screen {
public:
	Screen();
	bool init();
	void clean_exit();

	void render_frame();
	
	bool create_window();
	bool reload_window();

	int get_width();
	int get_height();

	void resize(int width, int height);
	float get_aspect_ratio() { return (float)width.as_int / (float)height.as_int; }

	void begin_gui();

	Camera* get_camera();
private:
	SDL_Window*   window;
	SDL_GLContext gl_context;

	SDL_Cursor*   gui_cursors[ImGuiMouseCursor_Count_];

	system_var width;
	system_var height;
	system_var fullscreen;
	system_var borderless;
	system_var field_of_view;
	system_var aspect_ratio;
	
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
extern Screen screen;

#endif