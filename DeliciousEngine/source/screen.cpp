#include "screen.h"
#include "screen_types.h"

#include <SDL.h>
#include <iostream>
#include "engine.h"
#include "console.h"
#include "world.h"
#include "build_info.h"
#include "default_shader.h"

#include <imgui.h>

#include <gtc/matrix_transform.hpp>

//@Temp
GLfloat bg_color[] = { 0.2f, 0.1f, 0.3f, 1.0f };

Screen::Screen(DeliciousEngine& engine) : System(engine) {
	window     = nullptr;
	gl_context = nullptr;

	vid_width         = 800;
	vid_height        = 600;
	vid_fullscreen    = false;
	vid_borderless    = false;
	vid_fov = 60.0f;

	gui_vao = gui_vbo = gui_ebo = 0;
	gui_shader_handle = 0;
	gui_texture_handle = 0;
	
	std::memset(gui_cursors, NULL, sizeof(gui_cursors));
}

bool Screen::load() {
	auto& console = engine.get<Console>();
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		console.printf("SDL could not be initialised: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ENGINE_GL_MAJOR);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ENGINE_GL_MINOR);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,  32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	console.register_variable("vid_width",      &vid_width,      CVAR_INT,   CVAR_CONFIG);
	console.register_variable("vid_height",     &vid_height,     CVAR_INT,   CVAR_CONFIG);
	console.register_variable("vid_fullscreen", &vid_fullscreen, CVAR_BOOL,  CVAR_CONFIG);
	console.register_variable("vid_borderless", &vid_borderless, CVAR_BOOL,  CVAR_CONFIG);
	console.register_variable("vid_fov",		&vid_fov,		 CVAR_FLOAT, CVAR_USER  );

	//camera.init(&field_of_view, &aspect_ratio);

	ImGui::CreateContext();
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();

	gui_cursors[ImGuiMouseCursor_Arrow]      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	gui_cursors[ImGuiMouseCursor_TextInput]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	gui_cursors[ImGuiMouseCursor_ResizeAll]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	gui_cursors[ImGuiMouseCursor_ResizeNS]   = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	gui_cursors[ImGuiMouseCursor_ResizeEW]   = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	gui_cursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	gui_cursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);

	return true;
}

bool Screen::free() {
	for (auto cursor : gui_cursors) {
		SDL_FreeCursor(cursor);
	}
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return true;
}

bool Screen::create_window() {
	auto& console = engine.get<Console>();
	//@Todo: Deprecate full fullscreen acquisition of GPU, instead use borderless fullscreen...
	//@Todo: Don't destroy gl context when changing resolution, instead render to framebuffer texture
	//		 and then display that as a scaled fullscreen quad.
	uint32 sdl_flags = SDL_WINDOW_OPENGL;
	if (vid_fullscreen.as_bool == true && vid_borderless.as_bool == true) {
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) == 0) {
			vid_width.as_int  = dm.w;
			vid_height.as_int = dm.h;
			sdl_flags |= SDL_WINDOW_BORDERLESS;
		}
		else {
			console.print("Cannot detect native resolution for borderless fullscreen, reverting to windowed mode.");
			vid_fullscreen = false;
			vid_borderless = false;
		}
	}
	else if (vid_fullscreen.as_bool == true) {
		sdl_flags |= SDL_WINDOW_FULLSCREEN;
	}
	else if (vid_borderless.as_bool == true) {
		sdl_flags |= SDL_WINDOW_BORDERLESS;
	}

	window = SDL_CreateWindow(
		DEFAULT_WIN_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		vid_width.as_int,
		vid_height.as_int,
		sdl_flags
	);
	if (window == nullptr) {
		console.printf("SDL window could not be created: %s", SDL_GetError());
		return false;
	}

	if (gl_context != nullptr) {
		if (SDL_GL_MakeCurrent(window, gl_context) != 0) {
			console.printf("Could not attach existing GL context to SDL window: %s", SDL_GetError());
			return false;
		}
		glViewport(0, 0, vid_width.as_int, vid_height.as_int);
	}
	else {
		gl_context = SDL_GL_CreateContext(window);
		if (gl_context == nullptr) {
			console.printf("SDL_GL context could not be created: %s", SDL_GetError());
			return false;
		}
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		return false;
	}

	glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//current_camera.calculate_projection(vid_fov.as_float, get_aspect_ratio());

	create_gui_objects();

	ortho_matrix = glm::ortho(0.0f, (float)vid_width.as_int, (float)vid_height.as_int, 0.0f);

	return true;
}

bool Screen::reload_window() {
	if (window != nullptr) {
		invalidate_gui_objects();
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	return create_window();
}

void Screen::render_frame() {
	auto& world = engine.get<World>();
	auto& console = engine.get<Console>();
	auto& screen = engine.get<Screen>();
	camera.update_projection(screen.width(), screen.height(), screen.aspect_ratio());
	world.draw();
	console.update_and_draw();

	draw_imgui();

	SDL_GL_SwapWindow(window);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Screen::resize(int new_width, int new_height) {
	vid_width = new_width;
	vid_height = new_height;
	reload_window();
}

void Screen::begin_gui() {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = vid_width.as_int;
	io.DisplaySize.y = vid_height.as_int;

	auto cursor = ImGui::GetMouseCursor();
	if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
		SDL_ShowCursor(0);
	}
	else {
		SDL_SetCursor(gui_cursors[cursor]);
		SDL_ShowCursor(1);
	}
	ImGui::NewFrame();
}

void Screen::draw_imgui() {
	ImGui::Render();
	auto draw_data = ImGui::GetDrawData();
	ImGuiIO& io = ImGui::GetIO();

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(gui_shader_handle);
	glUniform1i(gui_tex_uniform, 0);
	glUniformMatrix4fv(gui_proj_uniform, 1, GL_FALSE, &ortho_matrix[0][0]);
	glBindVertexArray(gui_vao);
	glBindTexture(GL_TEXTURE_2D, gui_texture_handle);
	glBindSampler(0, 0);

	for (int n = 0; n < draw_data->CmdListsCount; n++) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, gui_vbo);
		glBufferData(GL_ARRAY_BUFFER,
					 (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
					 (const GLvoid*)cmd_list->VtxBuffer.Data,
					 GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
					 (const GLvoid*)cmd_list->IdxBuffer.Data,
					 GL_STREAM_DRAW);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback) {
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x,
						  (int)(vid_height.as_int - pcmd->ClipRect.w),
						  (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
						  (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
							   sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
							   idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
}

#include "dgl.h"
void Screen::create_gui_objects() {
	gui_shader_handle = glCreateProgram();
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &gui_frag_shader, 0);
	if (!dgl::compile(vert, gui_vert_shader)) {
		GLchar buffer[1024];
		glGetShaderInfoLog(vert, 1024, NULL, buffer);
		std::cout << buffer << "\n";
	}
	if (!dgl::compile(frag, gui_frag_shader)) {
		GLchar buffer[1024];
		glGetShaderInfoLog(frag, 1024, NULL, buffer);
		std::cout << buffer << "\n";
	}
	glAttachShader(gui_shader_handle, vert);
	glAttachShader(gui_shader_handle, frag);
	glLinkProgram(gui_shader_handle);
	glDetachShader(gui_shader_handle, vert);
	glDetachShader(gui_shader_handle, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

	gui_tex_uniform = glGetUniformLocation(gui_shader_handle, "tex");
	gui_proj_uniform = glGetUniformLocation(gui_shader_handle, "proj_matrix");
	gui_pos_attrib = glGetAttribLocation(gui_shader_handle, "position");
	gui_texcoord_attrib = glGetAttribLocation(gui_shader_handle, "texcoord");
	gui_color_attrib = glGetAttribLocation(gui_shader_handle, "color");

	glGenBuffers(1, &gui_vbo);
	glGenBuffers(1, &gui_ebo);

	glGenVertexArrays(1, &gui_vao);
	glBindVertexArray(gui_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui_vbo);
	glEnableVertexAttribArray(gui_pos_attrib);
	glEnableVertexAttribArray(gui_texcoord_attrib);
	glEnableVertexAttribArray(gui_color_attrib);

	glVertexAttribPointer(gui_pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(gui_texcoord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(gui_color_attrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	glGenTextures(1, &gui_texture_handle);
	glBindTexture(GL_TEXTURE_2D, gui_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	io.Fonts->TexID = (void*)(intptr_t)gui_texture_handle;
}

void Screen::invalidate_gui_objects() {
	glDeleteVertexArrays(1, &gui_vao);
	glDeleteBuffers(1, &gui_vbo);
	glDeleteBuffers(1, &gui_ebo);
	gui_vao = gui_vbo = gui_ebo = 0;

	glDeleteProgram(gui_shader_handle);
	gui_shader_handle = 0;

	glDeleteTextures(1, &gui_texture_handle);
	ImGui::GetIO().Fonts->TexID = 0;
	gui_texture_handle = 0;
}