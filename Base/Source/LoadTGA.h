#ifndef LOAD_TGA_H
#define LOAD_TGA_H

struct Vector3;
GLuint LoadTGA(const char *file_path);

//This function directly edits a texture ID of a mesh at a single point. (Warning: Slightly expensive method)
//First parameter : Mesh's paint texture ID
//Second parameter : 0 <= x < 1, represents the x-coordinate to place the paint on, as a percentage value
//Third parameter : 0 <= x < 1, represents the x-coordinate to place the paint on, as a percentage value
//Fourth parameter : Color of paint
//Fifth parameter : Alpha of paint
//Sixth parameter : Length of texture ID (e.g for 256x256 texture size, put in 256)
GLuint PaintTGA(GLuint texture, float x, float y, Vector3 color, float alpha, float tgaLength);

//Same as PaintTGA but the paint spreads out a lot more
GLuint PaintTGABurst(GLuint texture, float x, float y, Vector3 color, float alpha, float tgaLength, int spread = 2);

GLuint NewTGA(int size);

#endif