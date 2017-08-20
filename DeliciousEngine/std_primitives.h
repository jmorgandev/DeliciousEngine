#ifndef DELICIOUS_PRIMITIVES_H
#define DELICIOUS_PRIMITIVES_H

#include "mesh_data.h"
#include <string>

//float gui_vertices[] = {
//	 0.0f,  1.0f,
//	 0.0f,  0.0f,
//	 1.0f,  1.0f,
//	 1.0f,  0.0f
//};
float gui_vertices[] = {
	0.0f,  0.0f,
	0.0f,  -1.0f,
	1.0f,  0.0f,
	1.0f,  -1.0f
};
float gui_texcoords[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};


const float std_triangle[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,

	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,

	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 0.5f, 1.0f
};
const float std_triangle_2d[] = {
	-0.5f, -0.5f,
	 0.5f, -0.5f,
	 0.0f,  0.5f,
	 	    
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 0.5f, 1.0f
};
const int std_triangle_vcount = 3;


const float std_quad[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,

	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f,

	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 1.0f, 1.0f,
	 0.0f, 0.0f,
	 1.0f, 1.0f,
	 0.0f, 1.0f
};
const float std_quad_2d[] = {
	-0.5f, -0.5f,
	 0.5f, -0.5f,
	 0.5f,  0.5f,
	-0.5f, -0.5f,
	 0.5f,  0.5f,	
	-0.5f,  0.5f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};
const int std_quad_vcount = 6;

#endif