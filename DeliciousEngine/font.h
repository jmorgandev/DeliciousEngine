#ifndef DELICIOUS_FONT_H
#define DELICIOUS_FONT_H

struct Mesh;
struct Texture;
struct Shader;

struct Font {
	GLuint gui_vao;
	Texture* texture;
	Shader* shader;
	int char_offset;
	int total_cells;
	int cell_height;
	int cell_width;
	int cell_rows;
	int cell_columns;
};

#endif