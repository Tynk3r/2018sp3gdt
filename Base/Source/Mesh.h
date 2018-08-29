#ifndef MESH_H
#define MESH_H

#include <string>
#include "Material.h"

#define MAX_TEXTURES 2

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	virtual ~Mesh();
	virtual void Render();
	void Render(unsigned offset, unsigned count);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;
	unsigned textureID;
	unsigned textureArray[MAX_TEXTURES];
	unsigned texturePaintID; //Texture ID for Paint, acts as a multitexture, but preferably kept away from the array used for multitexture
	float texCoordStretch; //(paint) amount to modify paint texture size in order to have accurate paint, this is dependent on texCoord
	float tgaLengthPaint; //(paint) amount to modify paint texture size in order to have accurate paint, this is dependent on paint texture id size
};

#endif