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

const MeshData primitive_cube = {
	/*Vertices*/ {
		//Front face
		{-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f},
		{-0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
		//Left face
		{-0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
		{-0.5f,-0.5f,-0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f,-0.5f},
		//Right face
		{ 0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f},
		{ 0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f,-0.5f},
		//Back face
		{-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f},
		{-0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
		//Top face
		{-0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f,-0.5f},
		{-0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
		//Bottom face
		{-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f,-0.5f},
		{-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f},
	},
	/*Normals*/ {
		{ 0.0f,0.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 0.0f,0.0f,1.0f },
		{ 0.0f,0.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 0.0f,0.0f,1.0f },

		{-1.0f,0.0f,0.0f },{-1.0f,0.0f,0.0f },{-1.0f,0.0f,0.0f },
		{-1.0f,0.0f,0.0f },{-1.0f,0.0f,0.0f },{-1.0f,0.0f,0.0f },

		{ 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f },
		{ 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f },

		{ 0.0f,0.0f,-1.0f},{ 0.0f,0.0f,-1.0f},{ 0.0f,0.0f,-1.0f},
		{ 0.0f,0.0f,-1.0f},{ 0.0f,0.0f,-1.0f},{ 0.0f,0.0f,-1.0f},

		{ 0.0f,1.0f,0.0f },{ 0.0f,1.0f,0.0f },{ 0.0f,1.0f,0.0f },
		{ 0.0f,1.0f,0.0f },{ 0.0f,1.0f,0.0f },{ 0.0f,1.0f,0.0f },

		{0.0f,-1.0f,0.0f},{0.0f,-1.0f,0.0f },{0.0f,-1.0f,0.0f },
		{0.0f,-1.0f,0.0f},{0.0f,-1.0f,0.0f },{0.0f,-1.0f,0.0f },
	},
	/*Texcoords*/ {
		{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f },
	}
};

#endif