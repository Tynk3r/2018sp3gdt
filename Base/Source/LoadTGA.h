#ifndef LOAD_TGA_H
#define LOAD_TGA_H

enum PAINT_PATTERNS
{
	PAINT_DOT, //single drop of paint
	PAINT_BURST, //'+' shape paint
	PAINT_UNIQUE_FIRE, //color cannot be changed
	PAINT_MAGICCIRCLE, //cool looking circle
	PAINT_TOTAL
};

struct Vector3;
GLuint LoadTGA(const char *file_path);

//This function directly edits a texture ID of a mesh at a single point. (Warning: Slightly expensive method)
//First parameter : Mesh's paint texture ID
//Second parameter : 0 <= x < 1, represents the x-coordinate to place the paint on, as a percentage value
//Third parameter : 0 <= x < 1, represents the x-coordinate to place the paint on, as a percentage value
//Fourth parameter : Color of paint
//Fifth parameter : Alpha of paint
//Sixth parameter : Length of texture ID (e.g for 256x256 texture size, put in 256)
//Seventh parameter : Type of paint shape you want
GLuint PaintTGA(GLuint texture, float x, float y, Vector3 color, float alpha, float tgaLength, PAINT_PATTERNS type);

GLuint NewTGA(int size);

#endif